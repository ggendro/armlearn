/**
 * @copyright Copyright (c) 2019
 */

#include "controller.h"

/**
 * @brief Construct a new Controller:: Controller object
 * 
 * @param port the port to use
 * @param baudrate to use, default : 115200
 */
Controller::Controller(const std::string& port, int baudrate, DisplayMode displayMode, std::ostream& out):mode(displayMode), output(out) {
    serialPort = new serial::Serial(port, baudrate);
    motors = new std::map<uint8_t, Servomotor*>();
}

/**
 * @brief Destroy the Controller:: Controller object
 * 
 */
Controller::~Controller(){
    delete serialPort;

    for(auto ptr = motors->begin(); ptr != motors->end(); ptr++){
        delete ptr->second;
    }
    delete motors;
}


/**
 * @brief Computes the checksum of a packet
 * 
 * @param data the data containedin the packet
 * @return uint8_t the according checksum
 */
uint8_t Controller::computeChecksum(const std::vector<uint8_t>& data){
    int sum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = data.cbegin(); ptr < data.cend(); ptr++){
		sum += *ptr;
	}

	return 255 - (sum % 256);
}

/**
 * @brief Checks if a status packet isvalid
 * 
 * @param packet the status packet to verify
 * @return true if valid
 * @return false if not
 */
bool Controller::validPacket(std::vector<uint8_t>& packet, int verifStep){

    switch(verifStep){ // Switch voluntarily does not have break statements to verify all following cases

        case 0: // Verify that packet is big enough to contain the required informations
            if (packet.size() < RESPONSE_BYTES) return false;

        case 1: // Verify first header
            if(packet[0] != PACKET_HEADER) return false;

        case 2: // Verify second header
            if(packet[1] != PACKET_HEADER) return false;

        case 3: // Verify that size is correct
            if(packet[3] != packet.size() - 4) return false;
        
        case 4: // Verify that no errors are raised
            if(packet[4] != 0) return false;

        case 5: // Verify that checksum is correct
            if(packet.back() != computeChecksum(std::vector<uint8_t>(packet.begin()+2, packet.end()-1))) return false;
            return true;

        default:
            return false;
    }

}


/**
 * @brief Send a packet to the connected serial port
 * 
 * @param buffer the content of the packet
 * @return int the number of bytes really sent
 * 
 * If controller display mode is superior or equal to print, will display the sent packet in the output stream
 */
int Controller::send(const std::vector<uint8_t>& buffer){

    std::vector<uint8_t> sendBuf(buffer);

    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);
    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);

    sendBuf.push_back(computeChecksum(buffer));

    int res = this->serialPort->write(sendBuf);

    if(mode > none){
        output << "Packet sent : ";
        for (auto&& x : sendBuf)
            output << (int) x << " ";

        output << "(" << res << ")" << std::endl;
    }

    return res;
}

/**
 * @brief Fills a buffer with the input from the device
 * 
 * @param buffer the buffer to fill data with
 * @param readAll if true, will read all available data in the buffer, if false, will only read bytesExoected bytes
 * @param wait if true, method will block until bytesExpected bytes are read or timeout is reached
 * @param bytesExpected the minimum number of bytes expected, if wait is true, will wait until this amount of bytes is in the buffer
 * @param timeout the time to wait if wait is true
 * @return int the number of bytes read
 * 
 * If controller display mode is superior or equal to print, will display the received packet in the output stream
 */
int Controller::receive(std::vector<uint8_t>& buffer, bool readAll, bool wait, int bytesExpected, int timeout){

    if(wait){
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

        while(serialPort->available() < bytesExpected && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < timeout){ // TODO: Implement without active waiting
            std::this_thread::sleep_for((std::chrono::milliseconds) timeout/10); // Will check every timeout/10 milliseconds
            currentTime = std::chrono::system_clock::now();
        }
    }


    int nbChar = bytesExpected;
    if(readAll) nbChar = serialPort->available();
    
    int res = serialPort->read(buffer, nbChar);

    if(mode > none){
        output << "Message received : ";
        for(auto&& v : buffer)
            output << (int) v << " ";

        output << "(" << res << ")" << std::endl;
    }

    return res;
}


/**
 * @brief Send a read instruction to a device
 * 
 * @param id the id of the servomotor to send the instruction to 
 * @param registerNum the address of the first register to read
 * @param nbRegisters the number of registers to read starting from registerNum
 * @return int the expected size of the status packet returned
 */
int Controller::readIns(uint8_t id, uint8_t registerNum, uint8_t nbRegisters){ // TODO: implement bulk read for MX series, to gain performance
    send({id, 0x04, READ_INSTRUCTION, registerNum, nbRegisters});
    return RESPONSE_BYTES + nbRegisters;
}

/**
 * @brief Send a write instruction to a device
 * 
 * @param id the id of the servomotor to send the instruction to 
 * @param startAddress the address of the first register to write in, careful, not all registers can be overwritten
 * @param newValues the values replacing the old ones, each value will overwrite the value of a regiser
 * @param wait if true, the servomotor will wait for an action command before taking the new value into account (ex: move the motor to a new position), InstructionRegistered register set to 1 during this waiting period
 * @return int the expected size of the status packet returned
 */
int Controller::writeIns(uint8_t id, uint8_t startAddress, const std::vector<uint8_t>& newValues, bool wait){ // TODO: implement syncWrite to gain performance
    uint8_t length = 3 + newValues.size();
    std::vector<uint8_t> toSend = {id, length};

    if(wait) 
        toSend.push_back(WRITE_WAIT_INSTRUCTION);
    else 
        toSend.push_back(WRITE_INSTRUCTION);
    
    toSend.push_back(startAddress);
    toSend.insert(toSend.end(), newValues.begin(), newValues.end());

    send(toSend);
    return RESPONSE_BYTES;
}

/**
 * @brief Execute instructions waiting for an action command in servomotors registers
 * 
 * @param ids the ids of the devices to send the action command to
 */
void Controller::execWaitingWrite(const std::vector<uint8_t>& ids){
    for(auto ptr = ids.cbegin(); ptr < ids.cend(); ptr++)
        send({*ptr, 0x02, ACTION_INSTRUCTION}); 
}

/**
 * @brief Function pattern repeated by most of execution commands
 * Composed of several steps:
 *  - 1. Get the Servomotor matching the id in parameter or handle error
 *  - 2. Send a packet
 *  - 3. Receive a packet or handle error
 *  - 4. Process received packet
 * 
 * @param id the id of the target servomotor
 * @param sendFunc function that send a packet and return the number of expected bytes in response, if 0: executionPattern returns immediatly false, if 1: executionPattern returns immediatly true , takes in parameter an iterator to the servomotor asked by the id
 * @param receiveFunc function that manages the response packet, takes in parameter the same iterator as sendFunc and the response packet
 * @return true if execution went well
 * @return false otherwise
 * @throw IdError if the id is incorrect
 * @throw if the response packet is incorrect
 */
bool Controller::executionPattern(uint16_t id, const std::function< int(std::map<uint8_t, Servomotor*>::iterator) >& sendFunc, const std::function< void(std::map<uint8_t, Servomotor*>::iterator, std::vector<uint8_t>&) >& receiveFunc){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){ // Change not valid if id is not present in the list
        std::stringstream disp;
        disp << "ID " << (int) id <<" not found.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw IdError(disp.str());
        return false;
    }

    if(ptr->second->getStatus() == offline){ // Change not valid if id is not present in the list
        std::stringstream disp;
        disp << "Device " << (int) id <<" not connected.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw ConnectionError(disp.str());
        return false;
    }

    int repSize = sendFunc(ptr); // Execute function that send packet and return the number of expectes bytes

    if(repSize == 0) return false;
    if(repSize == 1) return true;

    std::vector<uint8_t> rep;
    int res = receive(rep, false, true, repSize);

    if(res == repSize && validPacket(rep)){
        receiveFunc(ptr, rep); // Execute function that manage received packet
        return true;
    }

    std::stringstream disp;
    disp << "Incorrect response from device " << (int) id << " : ";
    for(auto&& v : rep) disp << v << " ";
    disp << "(" << res << ")";

    if(mode >= print) output << disp.str() << std::endl;
    if(mode >= except) throw ConnectionError(disp.str());
    return false;
    
}






/**
 * @brief Connect the controller to the physical devices
 * 
 * Connect to serial port and to all servomotors included in the controller
 */
void Controller::connect(){
    if(!serialPort->isOpen()) serialPort->open();
    serialPort->flush();


    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++){
        ping(ptr->first);
        ptr->second->setStatus(offline);
    }

    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++){
        std::vector<uint8_t> buf;
        int res = receive(buf, false, true, 10); // Expected answer is a packet with the following format: {header, header, id, length, error, modelNumberL, modelNumberH, firmwareVersion, id, checksum}

        if(res == 10 && validPacket(buf) && buf[2] == buf[8]){
            auto servo = motors->find(buf[8])->second;

            servo->setStatus(connected);
            servo->setModel(buf[5] + (buf[6] << BYTE_SIZE));
            servo->setFirmware(buf[7]);
        }
    }

}

/**
 * @brief Send a ping to a device, the device will return its id, model number and firmware version
 * 
 * @param id the id of the device to send the ping to
 */
void Controller::ping(uint8_t id){
    readIns(id, MODEL_REGISTER, MODEL_LENGTH); // Ask for id and model of the device
}


/**
 * @brief Add a servomotor to the controller's list
 * 
 * @param id the id of the servo
 * @param name the name of the servo
 * @return true if it was correctly added
 * @return false otherwise, if the id was already present
 */
bool Controller::addMotor(uint8_t id, const std::string& name, Type type){
    if(motors->find(id) != motors->end()){
        if(mode >= except) throw IdError("ID already taken.");
        return false;
    } 

    Servomotor* motor = new Servomotor(id, name, type);
    motors->insert(std::pair<int, Servomotor*>(id, motor));

    return true;
}

/**
 * @brief Remove a servomotor fro mthe controller's list
 * 
 * @param id the id of the servo
 * @return true if it was correctly removed
 * @return false otherwise, if it was not in the list
 */
bool Controller::removeMotor(uint8_t id){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){
        if(mode >= except) throw IdError("ID not found.");
        return false;
    } 

    delete ptr->second;
    motors->erase(ptr);

    return true;
}


/**
 * @brief Change the id of a servomotor
 * 
 * @param oldId the current id of the servomotor to change
 * @param newId the new id of the servomotor
 * @return true if the change succeeded
 * @return false otherwise
 */
bool Controller::changeId(uint8_t oldId, uint8_t newId){
    return executionPattern(oldId, 
        [this, oldId, newId](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(motors->find(newId) != motors->end()){ // Change not valid if new id is already in
                std::stringstream disp;
                disp << "New ID " << newId << " already existing.";

                if(mode >= print) output << disp.str() << std::endl;
                if(mode >= except) throw IdError(disp.str());
                return 0; 
            } 

            return writeIns(oldId, ID_REGISTER, {newId}); // Change id into the device
        },
        [this, newId](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){ // If change correctly executed in the device, change in the interface
            ptr->second->setId(newId); // Change in the servo class

            auto val = motors->extract(ptr); // Change in the list
            val.key() = newId;
            motors->insert(move(val));
        });
}

/**
 * @brief Turn the LED of the servomotor ON / OFF
 * 
 * @param id the id of the servomotor the LED must be changed
 * @param on if true, will turn LED on, if false will turn off
 * @return true if successfully changed
 * @return false otherwise
 */
bool Controller::turnLED(uint8_t id, bool on){
   return executionPattern(id, 
        [this, id, on](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(motors->find(id)->second->getLED() == on)
                return 1;

            return writeIns(id, LED_REGISTER, {(uint8_t) on});
        },
        [on](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setLED(on);
        });
}

/**
 * @brief Turn the LED of the servomotor ON if it is currently OFF and OFF if it is currently ON
 * 
 * @param id the id of the servomotor the LED must be changed
 * @return true if successfully changed
 * @return false otherwise
 */
bool Controller::turnLED(uint8_t id){
   bool on;

   return executionPattern(id, 
        [this, id, &on](std::map<uint8_t, Servomotor*>::iterator ptr){
            on = !ptr->second->getLED();

            return writeIns(id, LED_REGISTER, {(uint8_t) on});
        },
        [&on](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setLED(on);
        });
}

/**
 * @brief Change speed of the specified servomotor
 * 
 * @param id the id of the servomotor whose speed has to change
 * @param newSpeed the value of the new speed
 * @return true if successfully changed
 * @return false otherwise
 */
bool Controller::changeSpeed(uint8_t id, uint16_t newSpeed){
    return executionPattern(id, 
        [this, id, newSpeed](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(!ptr->second->validSpeed(newSpeed)){
                std::stringstream disp;
                disp << "Speed value " << newSpeed << " is out of the range.";

                if(mode >= print) output << disp.str() << std::endl;
                if(mode >= except) throw OutOfRangeError(disp.str());
                return 0; 
            }

            return writeIns(id, SPEED_REGISTER, {(uint8_t) newSpeed, (uint8_t)(newSpeed >> BYTE_SIZE)});
        },
        [newSpeed](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setTargetSpeed(newSpeed);
        });
}

/**
 * @brief Change speed for all servomotors in the list
 * 
 * @param newSpeed the value of the new speed
 */
void Controller::changeSpeed(uint16_t newSpeed){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){ 
        changeSpeed(ptr->first, newSpeed);
    }
}

/**
 * @brief Set the position of the servomotor
 * 
 * @param id the id of the servomotor whose position has to be changed
 * @param newPosition the new position of the servo
 * @return true if successfully changed
 * @return false otherwise
 */
bool Controller::setPosition(uint8_t id, uint16_t newPosition){
    return executionPattern(id, 
        [this, id, newPosition](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(!ptr->second->validPosition(newPosition)){
                std::stringstream disp;
                disp << "Position " << newPosition <<" is out of the range.";

                if(mode >= print) output << disp.str() << std::endl;
                if(mode >= except) throw OutOfRangeError(disp.str());
                
                return 0;
            }

            return writeIns(id, POSITION_REGISTER, {(uint8_t) newPosition, (uint8_t)(newPosition >> BYTE_SIZE)});
        },
        [newPosition](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setTargetPosition(newPosition);
        });
}

/**
 * @brief Set the position of all servomotors
 * 
 * @param newPosition the new position of the servo
 */
void Controller::setPosition(const std::vector<uint16_t>& newPosition){
    auto ptrPos = newPosition.cbegin();
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){ 
        setPosition(ptr->first, *ptrPos);
        ptrPos++;
    }
}

/**
 * @brief Set the position of the arm to backhoe position
 * 
 */
void Controller::goToBackhoe(){
    setPosition(BACKHOE_POSITION);
}

/**
 * @brief Set the position of the arm to sleep position
 * 
 */
void Controller::goToSleep(){
    setPosition(SLEEP_POSITION);
}


/**
 * @brief Add to the current target position
 * 
 * @param id the id of the servomotor whose position has to be changed
 * @param dx the number to add to the current goal position
 * @return true if successfully changed
 * @return false otherwise
 */
bool Controller::addPosition(uint8_t id, int dx){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){
        std::stringstream disp;
        disp << "ID " << id <<" not found.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw IdError(disp.str());
        return false;
    }

    return setPosition(id, ptr->second->getTargetPosition() + dx);
}

/**
 * @brief Add to the goal position of all servos
 * 
 * @param dx the number to add to the servos' position
 */
void Controller::addPosition(const std::vector<int> dx){
    auto ptrPos = dx.cbegin();
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){  
        addPosition(ptr->first, *ptrPos);
        ptrPos++;
    }
}


/**
 * @brief Check if the goal position of all servomotors has been reached
 * 
 * @param err the margin of error, if negative, will use the default value of servos
 * @return true 
 * @return false 
 */
bool Controller::goalReached(int err) const{
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){
        if(!ptr->second->targetPositionReached(err)) return false;
    }

    return true;
}


/**
 * @brief Ask information from servomotor device and update the values in the class representing it
 * 
 * @param id the id of the servomotor to update
 * @return true if information has successfully been updated
 * @return false otherwise
 */
bool Controller::updateInfos(uint8_t id){
    int repSize = readIns(id, READ_REGISTER, READ_LENGTH);
    std::vector<uint8_t> rep;
    int res = receive(rep, false, true, repSize);

    if(res == repSize && validPacket(rep)){
        motors->find(id)->second->setInfos(std::vector<uint8_t>(rep.begin()+5, rep.end()-1));
        return true;
    }

    std::stringstream disp;
    disp << "No response from device " << id << ".";

    if(mode >= print) output << disp.str() << std::endl;
    if(mode >= except) throw ConnectionError(disp.str());
    return false;
}

/**
 * @brief Update all servomotor informations (see updatesInfos(uint8_t id) for more details)
 * 
 */
void Controller::updateInfos(){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){
        updateInfos(ptr->first);
    }
    if(mode >= print) output << servosToString();
}


/**
 * @brief Return informations about servomotors under string format (see Servomotor::toString() method)
 * 
 * @return std::string informations contained by servomotors
 */
std::string Controller::servosToString() const {
    std::stringstream streamRep;
    streamRep << "Servomotors :" << std::endl;
    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++) streamRep << ptr->second->toString() << std::endl;

    return streamRep.str();
}
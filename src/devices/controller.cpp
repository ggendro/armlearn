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
Controller::Controller(const std::string& port, int baudrate) {
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
 */
int Controller::send(const std::vector<uint8_t>& buffer){

    std::vector<uint8_t> sendBuf(buffer);

    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);
    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);

    sendBuf.push_back(computeChecksum(buffer));

    int res = this->serialPort->write(sendBuf);

    //*
    std::cout << "Packet sent : ";
    for (auto&& x : sendBuf)
        std::cout << (int) x << " ";
    std::cout << "(" << res << ")" << std::endl;
    //*/

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

    //*
    std::cout << "Message received : ";
    for(auto&& v : buffer)
        std::cout << (int) v << " ";
    std::cout << "(" << res << ")" << std::endl;
    //*/

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
 * @brief Send a write insturction to a device
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

void Controller::execWaitingWrite(const std::vector<uint8_t>& ids){
    for(auto ptr = ids.cbegin(); ptr < ids.cend(); ptr++)
        send({*ptr, 0x02, ACTION_INSTRUCTION}); 
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
        ptr->second->setStatus(notConnected);
    }

    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++){
        std::vector<uint8_t> buf;
        int res = receive(buf, false, true, 10); // Expected answer is a packet with the following format: {header, header, id, length, error, modelNumberL, modelNumberH, firmwareVersion, id, checksum}

        if(res == 10 && validPacket(buf) && buf[2] == buf[8]){
            auto servo = motors->find(buf[8])->second;

            servo->setStatus(connected);
            servo->setModel(buf[5] + (buf[6] << 8));
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
bool Controller::addMotor(uint8_t id, const std::string& name){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++)
        if(ptr->first == id)
            return false;

    if(motors->find(id) != motors->end()) return false;

    Servomotor* motor = new Servomotor(id, name);
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
    if(ptr == motors->end()) return false;

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
    auto ptr = motors->find(oldId);
    if(ptr == motors->end() || motors->find(newId) != motors->end()) return false; // Change not valid if old id is not present in the list or if new id is already in

    int repSize = writeIns(oldId, ID_REGISTER, {newId}); // Change id into the device
    std::vector<uint8_t> rep;
    int res = receive(rep, false, true, repSize);

    if(res == repSize && validPacket(rep)){ // If change correctly executed in the device, change in the interface
        ptr->second->setId(newId); // Change in the servo class

        auto val = motors->extract(ptr); // Change in the list
        val.key() = newId;
        motors->insert(move(val));
        
        return true;
    }
    return false;
}

bool Controller::turnLED(uint8_t id, bool on){
    
}
bool Controller::turnLED(bool on){
    auto f = [on, *this](uint8_t id){return turnLED(id, on);};
    FOR_ALL_SERVOS( f, "LED not updated.")
}

bool changeSpeed(uint8_t id, uint16_t newSpeed);
bool changeSpeed(uint16_t newSpeed);
bool setPosition(uint8_t id, uint16_t newPosition);
bool setPosition(uint16_t newPosition);
bool addPosition(uint8_t id, uint16_t dx);
bool addPosition(uint16_t dx);


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
    return false;
}

/**
 * @brief Update all servomotor informations (see updatesInfos(uint8_t id) for more details)
 * 
 */
void Controller::updateInfos(){
    FOR_ALL_SERVOS(updateInfos, "Unable to update information from device.")
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
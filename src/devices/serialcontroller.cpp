/**
 * @copyright Copyright (c) 2019
 */

#include "serialcontroller.h"

using namespace armlearn;
using namespace communication;


SerialController::SerialController(const std::string& port, int baudrate, DisplayMode displayMode, std::ostream& out):AbstractController(displayMode, out) {
    serialPort = new serial::Serial(port, baudrate);
}

SerialController::~SerialController(){
    delete serialPort;
}


uint8_t SerialController::computeChecksum(const std::vector<uint8_t>& data){
    int sum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = data.cbegin(); ptr < data.cend(); ptr++){
		sum += *ptr;
	}

	return 255 - (sum % 256);
}

bool SerialController::validPacket(std::vector<uint8_t>& packet, int verifStep){

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


int SerialController::send(const std::vector<uint8_t>& buffer){

    std::vector<uint8_t> sendBuf(buffer);

    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);
    sendBuf.insert(sendBuf.begin(), PACKET_HEADER);

    sendBuf.push_back(computeChecksum(buffer));

    int res = this->serialPort->write(sendBuf);

    if(mode & print){
        output << "Packet sent : ";
        for (auto&& x : sendBuf)
            output << (int) x << " ";

        output << "(" << res << ")" << std::endl;
    }

    return res;
}

int SerialController::receive(std::vector<uint8_t>& buffer, bool readAll, bool wait, int bytesExpected, int sleepTime, int timeout){

    if(wait){
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

        while(serialPort->available() < bytesExpected && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < timeout){ // TODO: Implement without active waiting
            std::this_thread::sleep_for((std::chrono::milliseconds) sleepTime); // Will check every sleepTime milliseconds
            currentTime = std::chrono::system_clock::now();
        }
    }


    int nbChar = bytesExpected;
    if(readAll) nbChar = serialPort->available();
    
    int res = serialPort->read(buffer, nbChar);

    if(mode & print){
        output << "Message received : ";
        for(auto&& v : buffer)
            output << (int) v << " ";

        output << "(" << res << ")" << std::endl;
    }

    return res;
}


int SerialController::readIns(uint8_t id, uint8_t registerNum, uint8_t nbRegisters){ // TODO: implement bulk read for MX series, to gain performance
    send({id, 0x04, READ_INSTRUCTION, registerNum, nbRegisters});
    return RESPONSE_BYTES + nbRegisters;
}

int SerialController::writeIns(uint8_t id, uint8_t startAddress, const std::vector<uint8_t>& newValues, bool wait){ // TODO: implement syncWrite to gain performance
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

void SerialController::execWaitingWrite(const std::vector<uint8_t>& ids){
    for(auto ptr = ids.cbegin(); ptr < ids.cend(); ptr++)
        send({*ptr, 0x02, ACTION_INSTRUCTION}); 
}


bool SerialController::executionPattern(uint16_t id, const std::function< int(std::map<uint8_t, Servomotor*>::iterator) >& sendFunc, const std::function< void(std::map<uint8_t, Servomotor*>::iterator, std::vector<uint8_t>&) >& receiveFunc){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){ // Change not valid if id is not present in the list
        std::stringstream disp;
        disp << "ID " << (int) id << " not found.";

        if(mode & print) output << disp.str() << std::endl;
        if(mode & except) throw IdError(disp.str());
        return false;
    }

    if(ptr->second->getStatus() == offline){ // Change not valid if id is not present in the list
        std::stringstream disp;
        disp << "Device " << (int) id <<" not connected.";

        if(mode & print) output << disp.str() << std::endl;
        if(mode & except) throw ConnectionError(disp.str());
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

    if(mode & print) output << disp.str() << std::endl;
    if(mode & except) throw ConnectionError(disp.str());
    return false;
    
}






void SerialController::connect(){
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

    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++){
        if(ptr->second->getStatus() == connected && torqueEnabled(ptr->first)) ptr->second->setStatus(activated); 
    }
}


void SerialController::ping(uint8_t id){
    readIns(id, MODEL_REGISTER, MODEL_LENGTH); // Ask for id and model of the device
}



bool SerialController::changeId(uint8_t oldId, uint8_t newId){
    return executionPattern(oldId, 
        [this, oldId, newId](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(motors->find(newId) != motors->end()){ // Change not valid if new id is already in
                std::stringstream disp;
                disp << "New ID " << newId << " already existing.";

                if(mode & print) output << disp.str() << std::endl;
                if(mode & except) throw IdError(disp.str());
                return 0; 
            } 

            return writeIns(oldId, ID_REGISTER, {newId}); // Change id into the device
        },
        [this, newId](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){ // If change correctly executed in the device, change in the interface
            ptr->second->setId(newId); // Change in the servo class

            // Change in the list

            // C++11 version
            auto servo = ptr->second;
            motors->erase(ptr);
            motors->insert(std::pair<int, Servomotor*>(newId, servo));


            // C++17 version (without realloc)
            //auto val = motors->extract(ptr); 
            //val.key() = newId;
            //motors->insert(std::move(val));
        });
}

bool SerialController::turnLED(uint8_t id, bool on){
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

bool SerialController::turnLED(uint8_t id){
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


bool SerialController::changeSpeed(uint8_t id, uint16_t newSpeed){
    return executionPattern(id, 
        [this, id, newSpeed](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(!ptr->second->validSpeed(newSpeed)){
                std::stringstream disp;
                disp << "Speed value " << newSpeed << " is out of the range.";

                if(mode & print) output << disp.str() << std::endl;
                if(mode & except) throw OutOfRangeError(disp.str());
                return 0; 
            }

            return writeIns(id, SPEED_REGISTER, {(uint8_t) newSpeed, (uint8_t)(newSpeed >> BYTE_SIZE)});
        },
        [newSpeed](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setTargetSpeed(newSpeed);
        });
}


bool SerialController::setPosition(uint8_t id, uint16_t newPosition){
    return executionPattern(id, 
        [this, id, newPosition](std::map<uint8_t, Servomotor*>::iterator ptr){
            if(!ptr->second->validPosition(newPosition)){
                std::stringstream disp;
                disp << "Position " << newPosition <<" is out of the range.";

                if(mode & print) output << disp.str() << std::endl;
                if(mode & except) throw OutOfRangeError(disp.str());
                
                return 0;
            }

            return writeIns(id, POSITION_REGISTER, {(uint8_t) newPosition, (uint8_t)(newPosition >> BYTE_SIZE)});
        },
        [newPosition](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setTargetPosition(newPosition);
        });
}


bool SerialController::addPosition(uint8_t id, int dx){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){
        std::stringstream disp;
        disp << "ID " << id <<" not found.";

        if(mode & print) output << disp.str() << std::endl;
        if(mode & except) throw IdError(disp.str());
        return false;
    }

    return setPosition(id, ptr->second->getTargetPosition() + dx);
}


bool SerialController::enableTorque(int id, bool enable){
    return executionPattern(id, 
        [this, id, enable](std::map<uint8_t, Servomotor*>::iterator ptr){
            return writeIns(id, TORQUE_REGISTER, {(uint8_t) enable});
        },
        [enable](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setStatus(enable ? activated : connected);
        });
}

bool SerialController::torqueEnabled(int id){
    bool isEnabled;
    bool execFine = executionPattern(id, 
        [this, id](std::map<uint8_t, Servomotor*>::iterator ptr){
            return readIns(id, TORQUE_REGISTER, 1);
        },
        [&isEnabled](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            isEnabled = rep[5];
        });

        if(!execFine){
            std::stringstream disp;
            disp << "Error during execution. Cannot read torque value of device " << id << ".";

            if(mode & print) output << disp.str() << std::endl;
            if(mode & except) throw ConnectionError(disp.str());
            return false;
        }

        return isEnabled;
}


bool SerialController::updateInfos(uint8_t id){
    return executionPattern(id, 
        [this, id](std::map<uint8_t, Servomotor*>::iterator ptr){
            return readIns(id, READ_REGISTER, READ_LENGTH);
        },
        [this, id](std::map<uint8_t, Servomotor*>::iterator ptr, std::vector<uint8_t>& rep){
            ptr->second->setInfos(std::vector<uint8_t>(rep.begin()+5, rep.end()-1));

            if(torqueEnabled(ptr->first))
                ptr->second->setStatus(activated);
            else
                ptr->second->setStatus(connected);
        });
}
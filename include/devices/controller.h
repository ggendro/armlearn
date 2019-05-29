/**
 * @file controller.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the Controller class, used for communication with hardware
 * @version 0.1
 * @date 2019-05-28
 * 
 * Documenation about the communication protocol can be found at http://support.robotis.com/en/product/actuator/dynamixel/communication/dxl_packet.htm
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <serial/serial.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "servomotor.h"
#include "connectionerror.h"


// Default baudrate for the serial port
#define DEFAULT_BAUDRATE 115200

// Header of a packet
#define PACKET_HEADER 0xFF
// Default max delay allowed when waiting for a response from a device
#define RESPONSE_DELAY 1000
// Default min bytes expected when waiting for a packet from a device
#define RESPONSE_BYTES 6
// Id to use for broadcast a packet to all devices (careful, no responses are returned when broadcast is used)
#define BROADCAST_ID 0xFE

// Read instruction for sending packet
#define READ_INSTRUCTION 0x02
// Write instruction for sending packet
#define WRITE_INSTRUCTION 0x03
// Write instruction with wait for action command
#define WRITE_WAIT_INSTRUCTION 0x04
// Action command, execute instructions sent with WRITE_WAIT_INSTRUCTION
#define ACTION_INSTRUCTION 0x05

// Macro executing a function for all servomotors and displaying an error message if it did not succeeded
// func has to take an uint8_t as parameter and return a boolean
// message is an error message under string format
#define FOR_ALL_SERVOS(func, message) for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){ if(!func(ptr->first)) std::cerr << "Error for ID " << (int) ptr->first << " : " << message << std::endl; }

/**
 * @class Controller
 * @brief Provides an interface to link servomotor classes with servomotor devices using serial port
 * 
 */
class Controller{

    private:
        serial::Serial* serialPort;
        std::map<uint8_t, Servomotor*>* motors;

        static uint8_t computeChecksum(const std::vector<uint8_t>& data);
        static bool validPacket(std::vector<uint8_t>& packet, int verifStep = 0);

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer, bool readAll = false, bool wait = false, int bytesExpected = RESPONSE_BYTES, int timeout = RESPONSE_DELAY);

        int readIns(uint8_t id, uint8_t registerNum, uint8_t nbRegisters);
        int writeIns(uint8_t id, uint8_t startAddress, const std::vector<uint8_t>& newValues, bool wait = false);
        void execWaitingWrite(const std::vector<uint8_t>& ids);


    public:
        Controller(const std::string& port, int baudrate = DEFAULT_BAUDRATE);
        ~Controller();

        void connect();
        void ping(uint8_t id);

        bool addMotor(uint8_t id, const std::string& name);
        bool removeMotor(uint8_t id);

        bool changeId(uint8_t oldId, uint8_t newId);
        bool turnLED(uint8_t id, bool on);
        bool turnLED(bool on);
        bool changeSpeed(uint8_t id, uint16_t newSpeed);
        bool changeSpeed(uint16_t newSpeed);
        bool setPosition(uint8_t id, uint16_t newPosition);
        bool setPosition(uint16_t newPosition);
        bool addPosition(uint8_t id, uint16_t dx);
        bool addPosition(uint16_t dx);

        bool updateInfos(uint8_t);
        void updateInfos();

        std::string servosToString() const;
    
};

#endif
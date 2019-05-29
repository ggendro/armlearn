/**
 * @file controller.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the Controller class, used for communication with hardware
 * @version 0.1
 * @date 2019-05-28
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
        //int writeIns();
        //int syncWriteIns();


    public:
        Controller(const std::string& port, int baudrate = DEFAULT_BAUDRATE);
        ~Controller();

        void connect();
        void ping(uint8_t id);

        bool addMotor(uint8_t id, const std::string& name);
        bool removeMotor(uint8_t id);

        bool changeId(uint8_t oldId, uint8_t newId);
        bool turnLED(bool on);
        bool changeSpeed(uint16_t newSpeed);
        bool setPosition(uint16_t newPosition);
        bool addPosition(uint16_t dx);

        bool updateInfos(uint8_t id);
        void updateInfos();

        std::string servosToString() const;
    
};

#endif
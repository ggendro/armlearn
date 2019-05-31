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
#include <functional>

#include "servomotor.h"
#include "connectionerror.h"
#include "iderror.h"
#include "outofrangeerror.h"


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

// Position of the servomotors to put the arm in backhoe position
#define BACKHOE_POSITION {2048, 2048, 2048, 2048, 512, 256}
// Position of the servomotors to put the arm in sleep position
#define SLEEP_POSITION {2048, 1025, 1025, 1830, 512, 256}


/**
 * @brief Controller display mode, ordered from the one giving the least information to user to the one giving the most
 * 
 *  - none : No information display
 *  - print : Print when sending and receiving packet and when handling errors
 *  - except : Print and throw exceptions when errors occur
 */
enum DisplayMode{
    none,
    print,
    except
};


/**
 * @class Controller
 * @brief Provides an interface to link servomotor classes with servomotor devices using serial port
 * 
 */
class Controller{

    private:
        serial::Serial* serialPort;
        std::map<uint8_t, Servomotor*>* motors;

        DisplayMode mode;
        std::ostream& output;

        static uint8_t computeChecksum(const std::vector<uint8_t>& data);
        static bool validPacket(std::vector<uint8_t>& packet, int verifStep = 0);

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer, bool readAll = false, bool wait = false, int bytesExpected = RESPONSE_BYTES, int timeout = RESPONSE_DELAY);

        int readIns(uint8_t id, uint8_t registerNum, uint8_t nbRegisters);
        int writeIns(uint8_t id, uint8_t startAddress, const std::vector<uint8_t>& newValues, bool wait = false);
        void execWaitingWrite(const std::vector<uint8_t>& ids);

        bool executionPattern(uint16_t id, const std::function< int(std::map<uint8_t, Servomotor*>::iterator) >& sendFunc, const std::function< void(std::map<uint8_t, Servomotor*>::iterator, std::vector<uint8_t>&) >& receiveFunc);


    public:
        Controller(const std::string& port, int baudrate = DEFAULT_BAUDRATE, DisplayMode displayMode = except, std::ostream& out = std::cout);
        ~Controller();

        void connect();
        void ping(uint8_t id);

        bool addMotor(uint8_t id, const std::string& name, Type type);
        bool removeMotor(uint8_t id);

        bool changeId(uint8_t oldId, uint8_t newId);
        bool turnLED(uint8_t id, bool on);
        bool turnLED(uint8_t id);

        bool changeSpeed(uint8_t id, uint16_t newSpeed);
        void changeSpeed(uint16_t newSpeed);

        bool setPosition(uint8_t id, uint16_t newPosition);
        void setPosition(const std::vector<uint16_t>& newPosition);
        void goToBackhoe();
        void goToSleep();

        bool addPosition(uint8_t id, int dx);
        void addPosition(const std::vector<int> dx);

        bool goalReached(int err = POSITION_ERROR_MARGIN) const;

        bool updateInfos(uint8_t id);
        void updateInfos();

        std::string servosToString() const;
    
};

#endif
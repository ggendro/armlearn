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
#include <math.h>

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


        /**
         * @brief Computes the checksum of a packet
         * 
         * @param data the data containedin the packet
         * @return uint8_t the according checksum
         */
        static uint8_t computeChecksum(const std::vector<uint8_t>& data);

        /**
         * @brief Checks if a status packet isvalid 
         * 
         * @param packet the status packet to verify
         * @param verifStep the current step of verification, 0 means that no step has been done, 5 means that only one step is remaining
         * @return true if valid
         * @return false if not 
         */
        static bool validPacket(std::vector<uint8_t>& packet, int verifStep = 0);


        /**
         * @brief Sends a packet to the connected serial port
         * 
         * @param buffer the content of the packet
         * @return int the number of bytes really sent
         * 
         * If controller display mode is superior or equal to print, will display the sent packet in the output stream
         */
        int send(const std::vector<uint8_t>& buffer);

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
        int receive(std::vector<uint8_t>& buffer, bool readAll = false, bool wait = false, int bytesExpected = RESPONSE_BYTES, int timeout = RESPONSE_DELAY);


        /**
         * @brief Sends a read instruction to a device
         * 
         * @param id the id of the servomotor to send the instruction to 
         * @param registerNum the address of the first register to read
         * @param nbRegisters the number of registers to read starting from registerNum
         * @return int the expected size of the status packet returned
         */
        int readIns(uint8_t id, uint8_t registerNum, uint8_t nbRegisters);

        /**
         * @brief Sends a write instruction to a device
         * 
         * @param id the id of the servomotor to send the instruction to 
         * @param startAddress the address of the first register to write in, careful, not all registers can be overwritten
         * @param newValues the values replacing the old ones, each value will overwrite the value of a regiser
         * @param wait if true, the servomotor will wait for an action command before taking the new value into account (ex: move the motor to a new position), InstructionRegistered register set to 1 during this waiting period
         * @return int the expected size of the status packet returned
         */
        int writeIns(uint8_t id, uint8_t startAddress, const std::vector<uint8_t>& newValues, bool wait = false);

        /**
         * @brief Execute instructions waiting for an action command in servomotors registers
         * 
         * @param ids the ids of the devices to send the action command to
         */
        void execWaitingWrite(const std::vector<uint8_t>& ids);


        /**
         * @brief Function pattern repeated by most of execution commands
         * Composed of several steps:
         *  - 1. Get the Servomotor matching the id in parameter or handle error
         *  - 2. Send a packet
         *  - 3. Receive a packet or handle error
         *  - 4. Process received packet
         * 
         * @param id the id of the target servomotor
         * @param sendFunc function that send a packet and return the number of expected bytes in response, if 0: executionPattern returns immediatly false, if 1: executionPattern returns immediatly true, takes in parameter an iterator to the servomotor asked by the id
         * @param receiveFunc function that manages the response packet, takes in parameter the same iterator as sendFunc and the response packet
         * @return true if execution went well
         * @return false otherwise
         * @throw IdError if the id is incorrect
         * @throw if the response packet is incorrect
         */
        bool executionPattern(uint16_t id, const std::function< int(std::map<uint8_t, Servomotor*>::iterator) >& sendFunc, const std::function< void(std::map<uint8_t, Servomotor*>::iterator, std::vector<uint8_t>&) >& receiveFunc);


    public:

        /**
         * @brief Construct a new Controller object
         * 
         * @param port the port to use
         * @param baudrate to use, default : 115200
         * @param displayMode mode of display (see DisplayMode enum for more details)
         * @param out the output stream to display the results, standard std output by default
         */
        Controller(const std::string& port, int baudrate = DEFAULT_BAUDRATE, DisplayMode displayMode = except, std::ostream& out = std::cout);

        /**
         * @brief Destroys the Controller:: Controller object
         * 
         */
        ~Controller();


        
        /**
         * @brief Connects the controller to the physical devices
         * 
         * Connect to serial port and to all servomotors included in the controller
         */
        void connect();


        /**
         * @brief Sends a ping to a device, the device will return its id, model number and firmware version
         * 
         * @param id the id of the device to send the ping to
         */
        void ping(uint8_t id);



        /**
         * @brief Adds a servomotor to the controller's list
         * 
         * @param id the id of the servo
         * @param name the name of the servo
         * @return true if it was correctly added
         * @return false otherwise, if the id was already present
         */
        bool addMotor(uint8_t id, const std::string& name, Type type);

        /**
         * @brief Removes a servomotor fro mthe controller's list
         * 
         * @param id the id of the servo
         * @return true if it was correctly removed
         * @return false otherwise, if it was not in the list
         */
        bool removeMotor(uint8_t id);

        /**
         * @brief Change the id of a servomotor
         * 
         * @param oldId the current id of the servomotor to change
         * @param newId the new id of the servomotor
         * @return true if the change succeeded
         * @return false otherwise
         */
        bool changeId(uint8_t oldId, uint8_t newId);

        /**
         * @brief Turns the LED of the servomotor ON / OFF
         * 
         * @param id the id of the servomotor the LED must be changed
         * @param on if true, will turn LED on, if false will turn off
         * @return true if successfully changed
         * @return false otherwise
         */
        bool turnLED(uint8_t id, bool on);

        /**
         * @brief Turns the LED of the servomotor ON if it is currently OFF and OFF if it is currently ON
         * 
         * @param id the id of the servomotor the LED must be changed
         * @return true if successfully changed
         * @return false otherwise
         */
        bool turnLED(uint8_t id);


        /**
         * @brief Changes speed of the specified servomotor
         * 
         * @param id the id of the servomotor whose speed has to change
         * @param newSpeed the value of the new speed
         * @return true if successfully changed
         * @return false otherwise
         */
        bool changeSpeed(uint8_t id, uint16_t newSpeed);

        /**
         * @brief Changes speed for all servomotors in the list
         * 
         * @param newSpeed the value of the new speed
         */
        void changeSpeed(uint16_t newSpeed);


        /**
         * @brief Sets the position of the servomotor
         * 
         * @param id the id of the servomotor whose position has to be changed
         * @param newPosition the new position of the servo
         * @return true if successfully changed
         * @return false otherwise
         */
        bool setPosition(uint8_t id, uint16_t newPosition);

        /**
         * @brief Sets the position of all servomotors
         * 
         * @param newPosition the new position of the servo
         */
        void setPosition(const std::vector<uint16_t>& newPosition);

        /**
         * @brief Sets the position of the arm to backhoe position
         * 
         */
        void goToBackhoe();

        /**
         * @brief Sets the position of the arm to sleep position
         * 
         */
        void goToSleep();


        /**
         * @brief Adds to the current target position
         * 
         * @param id the id of the servomotor whose position has to be changed
         * @param dx the number to add to the current goal position
         * @return true if successfully changed
         * @return false otherwise
         */
        bool addPosition(uint8_t id, int dx);

        /**
         * @brief Adds to the goal position of all servos
         * 
         * @param dx the number to add to the servos' position
         */
        void addPosition(const std::vector<int> dx);


        /**
         * @brief Enables or disables a servomotor's ability to move or hold a position (torque)
         * 
         * @param enable if true, enables the torque, otherwise, disables it
         * @return true if successfully changed
         * @return false otherwise
         */
        bool enableTorque(int id, bool enable = true);

        /**
         * @brief Checks if torque is enabled for the given servomotor (see enableTorque() method)
         * 
         * @param id the id of the torque to check
         * @return true if enabled
         * @return false otherwise
         */
        bool torqueEnabled(int id);


        /**
         * @brief Checks if the goal position of all servomotors has been reached (i.e: all servomotors have stopped moving)
         * 
         * @return true if all motors stopped
         * @return false otherwise
         */
        bool goalReached() const;

        /**
         * @brief Returns the sum squared error of the differences between servo's target and current positions
         * 
         * @return double the SSE
         */
        double positionSumSquaredError() const;


        /**
         * @brief Asks information from servomotor device and update the values in the class representing it
         * 
         * @param id the id of the servomotor to update
         * @return true if information has successfully been updated
         * @return false otherwise
         */
        bool updateInfos(uint8_t id);

        /**
         * @brief Updates all servomotor informations (see updatesInfos(uint8_t id) for more details)
         * 
         */
        void updateInfos();


        /**
         * @brief Returns informations about servomotors under string format (see Servomotor::toString() method)
         * 
         * @return std::string informations contained by servomotors
         */
        std::string servosToString() const;
    
};

#endif
/**
 * @file servomotor.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the servomotor class
 * @version 0.1
 * @date 2019-05-28
 * 
 * Documentation about the servomotor registers can be found at http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-64at_ar.htm
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib>

// Starting address of the register containing the model number
#define MODEL_REGISTER 0x00
// Number of successive registers containging information about model number, firmware version and ID
#define MODEL_LENGTH 0x04 

// Starting address of the register containing the read-only information about the state of the servomotor
#define READ_REGISTER 0x24
// Number of successive registers containging read-only information
#define READ_LENGTH 0x0A

// Address of the register containing the ID of the servomotor
#define ID_REGISTER 0x03
// Address of the register containing the status of the LED
#define LED_REGISTER 0x19

// Address of the starting register containing the target speed value (Speed value is registered on 2 bytes)
#define SPEED_REGISTER 0x20

// Address of the starting register containing the target position value (Position value is registered on 2 bytes)
#define POSITION_REGISTER 0x1E

// Size of a byte in servomotor register
#define BYTE_SIZE 8

// Margin of error between the current position and the target position
#define POSITION_ERROR_MARGIN 5


/**
 * @brief Enum representing the status of the servomotor
 * 
 *  - offline: not connected to the physical device
 *  - connected: connected to the physical device but motors are not allowed to move or maintain a position
 *  - activated: connected,  motors can receive move orders and are able to maintain a position TODO: implement (for now, merged with connected)
 */
enum Status{
    offline,
    connected,
    activated
};


/**
 * @class Servomotor
 * @brief Represents a servomotor
 * 
 */
class Servomotor{

    private:
        uint8_t id;
        std::string name;
        Status status;

        uint16_t modelNum;
        uint8_t firmware;

        uint16_t targetSpeed;
        uint16_t targetPosition;

        uint16_t position;
        uint16_t speed;
        uint16_t load;
        uint8_t voltage;
        uint8_t temperature;

        bool instructionregistered;
        bool inMovement;

        bool activeLED;

        std::chrono::time_point<std::chrono::system_clock> creationTime;
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        

    public:
        Servomotor(uint8_t id, const std::string& name);
        ~Servomotor();

        uint8_t getId() const;
        std::string getName() const;
        Status getStatus() const;
        bool getLED() const;
        uint16_t getTargetPosition() const;

        std::string toString() const;

        void setStatus(Status stat);
        void setModel(uint16_t mod);
        void setFirmware(uint8_t firm);
        void setId(uint8_t id);
        void setName(const std::string& newName);
        void setLED(bool on);
        void setInfos(const std::vector<uint8_t>& infos);
        void setTargetSpeed(uint16_t speed);
        void setTargetPosition(uint16_t position);

        bool validSpeed(uint16_t speed) const;
        bool validPosition(uint16_t position) const;
        bool targetPositionReached(uint16_t err = -1) const;

};

#endif
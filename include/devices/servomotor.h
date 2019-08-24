/**
 * @file servomotor.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
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

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib>

#include "typeerror.h"
#include "range.h"


// Starting address of the register containing the model number
#define MODEL_REGISTER 0x00
// Number of successive registers containging information about model number, firmware version and ID
#define MODEL_LENGTH 0x04 

// Starting address of the register containing the read-only information about the state of the servomotor
#define READ_REGISTER 0x24
// Number of successive registers containging read-only information
#define READ_LENGTH 0x0B

// Address of the register containing the ID of the servomotor
#define ID_REGISTER 0x03
// Address of the register containing the status of the LED
#define LED_REGISTER 0x19

// Address of the starting register containing the target speed value (Speed value is registered on 2 bytes)
#define SPEED_REGISTER 0x20

// Address of the starting register containing the target position value (Position value is registered on 2 bytes)
#define POSITION_REGISTER 0x1E

// Address of the register containing the status of the motors, enabled or disabled
#define TORQUE_REGISTER 0x18

// Size of a byte in servomotor register
#define BYTE_SIZE 8

// Error value to add to the position error if the servomotor is still moving while checking if target position is reached 
#define ERROR_MOVING 15



/**
 * @brief Enum representing the status of the servomotor
 * 
 *  - offline: not connected to the physical device
 *  - connected: connected to the physical device but motors cannot maintain a position, will turn to activated mode if receives a move request
 *  - activated: connected + motors can receive move orders and are able to maintain a position
 */
enum Status{
    offline,
    connected,
    activated
};


/**
 * @brief Enum representing the type of the servomotor, used for range setting
 * 
 */
enum Type{
    base,
    shoulder, 
    elbow, 
    wristAngle, 
    wristRotate, 
    gripper
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
        Type type;

        uint16_t posMin;
        uint16_t posMax;

        uint16_t modelNum;
        uint8_t firmware;

        uint16_t targetSpeed;
        uint16_t targetPosition;

        uint16_t position;
        uint16_t speed;
        uint16_t load;
        uint8_t voltage;
        uint8_t temperature;

        bool instructionRegistered;
        bool inMovement;

        bool activeLED;

        std::chrono::time_point<std::chrono::system_clock> creationTime;
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        

    public:

        /**
         * @brief Constructs a new Servomotor:: Servomotor object
         * 
         * @param id the id of the servomotor
         * @param name the name of theservomotor
         */
        Servomotor(uint8_t id, const std::string& name, Type type);

        /**
         * @brief Destroys the Servomotor:: Servomotor object
         * 
         */
        ~Servomotor();


        /**
         * @brief Returns the id of the servomotor
         * 
         * @return int the id with int format
         */
        uint8_t getId() const;

        /**
         * @brief Returns the name of the servomotor
         * 
         * @return std::string the name with string format
         */
        std::string getName() const;

        /**
         * @brief Returns the current status of the servomotor (connected or not)
         * 
         * @return Status an enum corresponding to the status of the servomotor device
         */
        Status getStatus() const;

        /**
         * @brief Returns the type of the servomotor
         * 
         * @return Type an enum corresponding to the type of the servomotor
         */
        Type getType() const;

        /**
         * @brief Returns the current state of the servomotor's LED
         * 
         * @return true if ON
         * @return false if OFF
         */
        bool getLED() const;

        /**
         * @brief Get the current target speed
         * 
         * @return uint16_t the speed to reach
         */
        uint16_t getTargetSpeed() const;

        /**
         * @brief Returns the current target position
         * 
         * @return uint16_t the position to reach
         */
        uint16_t getTargetPosition() const;


        /**
         * @brief Get the Current real position
         * 
         * @return uint16_t the real position of the servo
         */
        uint16_t getCurrentPosition() const;

        /**
         * @brief Get the Time since last update
         * 
         * @return double the time in seconds
         */
        double getTimeSinceUpdate() const; // TODO: add a parameter to change unit used


        /**
         * @brief Returns a string containing informations about the servomotor (id, name, status, position, ...)
         * 
         * @return std::string informations contained in the servomotor under string format
         */
        std::string toString() const;



        /**
         * @brief Changes the name of the servo
         * 
         * @param newName the new name to set
         */
        void setName(const std::string& newName);

        /**
         * @brief Sets the Status of the servomotor (see Status enum for more details)
         * 
         * @param stat the new status to set
         */
        void setStatus(Status stat);

        /**
         * @brief Sets the Type of the servomotor (see Type enum for more details), adapts the ranges to the new type
         * 
         * @param newType the new type to set
         */
        void setType(Type newType);

        /**
         * @brief Sets model number of the servomotor
         * 
         * @param mod the model number to set
         */
        void setModel(uint16_t mod);

        /**
         * @brief Sets firmware version of the servomotor
         * 
         * @param firm the firmware version to set
         */
        void setFirmware(uint8_t firm);

        /**
         * @brief Sets the id of the servomotor
         * 
         * @param id the new id to set
         */
        void setId(uint8_t id);

        /**
         * @brief Changes the value of the LED
         * 
         * @param on the value of the LED (true = ON, false = OFF)
         */
        void setLED(bool on);

        /**
         * @brief Sets read-only informations of the servomotor
         * 
         * @param infos the infos to set
         * 
         * Informations set:
         *  - Present position
         *  - Current speed
         *  - Current load
         *  - Current voltage
         *  - Current temperature
         *  - Is an instruction registered in the servo
         *  - Is the servo currently in movement
         */
        void setInfos(const std::vector<uint8_t>& infos);

        /**
         * @brief Sets the target speed of the servomotor
         * 
         * @param speed the new speed to reach
         */
        void setTargetSpeed(uint16_t speed);

        /**
         * @brief Sets the target position of the servomotor
         * 
         * @param speed the new position to reach
         */
        void setTargetPosition(uint16_t position);



        /**
         * @brief Checks if the speed is within the boundaries of the servomotor
         * 
         * @return true if it is
         * @return false otherwise
         */
        bool validSpeed(uint16_t speed) const;

        /**
         * @brief Checks if the position is within the boundaries of the servomotor
         * 
         * @return true if it is
         * @return false otherwise
         */
        bool validPosition(uint16_t position) const;

        /**
         * @brief Checks if the position is within the boundaries of the servomotor and if not not set it to the closest boundary
         * 
         * @return uint16_t the scaled position
         */
        uint16_t toValidPosition(uint16_t position) const;
        
        /**
         * @brief Rescale the position to match the boundaries of the servomotor
         * 
         * @param position the position to scale
         * @param oldMin the min value of the position
         * @param oldMax the max value of the position
         * @return uint16_t the scaled position
         */
        uint16_t scalePosition(double position, double oldMin=-1, double oldMax=1) const;



        /**
         * @brief Cheks if the servomotor is currently moving
         * 
         * @return true if it is still moving
         * @return false otherwise
         */
        bool motorMoving() const;

        /**
         * @brief Checks if the target position has been reached
         * 
         * @return int the difference between the target position and the current position, adds ERROR_MOVING to the result if the servomotor is still moving
         */
        int targetPositionReached() const;


    friend class DeviceLearner;
};



#endif
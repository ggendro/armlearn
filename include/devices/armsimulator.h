/**
 * @file armsimulator.h
 * @author Gaël Gendron
 * @brief File containing the ArmSimulator class, inherited from AbstractContoller, used to control a simulation of the hardware
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ARMSIMULATOR_H
#define ARMSIMULATOR_H

#include <vector>
#include <map>
#include <iostream>

#include "abstractcontroller.h"
#include "servomotor.h"
#include "converter.h"

namespace armlearn {
    namespace communication{
        

// Constant values for physical measures from device (waiting for a real simulation)
#define CURRENT_TEMP 37
#define CURRENT_LOAD 1000
#define CURRENT_VOLTAGE 120
#define INSTRUCTION_WAITING 0

// Acceleration factor to pass time faster
#define TIME_MUL 1000


/**
 * @class ArmSimulator
 * @brief Provides an interface to link servomotor classes with simulated servomotors
 * 
 */
class ArmSimulator : public AbstractController{

    protected:

        /**
         * @brief Gets a pointer to the wanted servomotor
         * 
         * @param id the ID of the servomotor to look for
         * @param ptr pointer to the servomotor
         * @return true if the servomotor with  the specified id is found
         * @return false otherwise
         */
        bool getMotor(uint8_t id, Servomotor*& ptr);

    public:

        /**
         * @brief Construct a new ArmSimulator object
         * 
         * @param displayMode mode of display (see DisplayMode enum for more details)
         * @param out the output stream to display the results, standard std output by default
         */
        ArmSimulator(DisplayMode displayMode = except, std::ostream& out = std::cout);

        /**
         * @brief Destroys the ArmSimulator object
         * 
         */
        ~ArmSimulator();


        
        /**
         * @brief Connects the controller to the devices
         * 
         * Enables all servomotors
         */
        virtual void connect() override;


        /**
         * @brief Returns id of the device, used in Controllers with physical devices, not useful in simulator controller
         * 
         * @param id the id of the device to send the ping to
         * 
         * Inherited method from AbstractController
         */
        virtual void ping(uint8_t id) override;

        /**
         * @brief Changes the id of a servomotor
         * 
         * @param oldId the current id of the servomotor to change
         * @param newId the new id of the servomotor
         * @return true if the change succeeded
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        virtual bool changeId(uint8_t oldId, uint8_t newId) override;

        /**
         * @brief Turns the LED of the servomotor ON / OFF
         * 
         * @param id the id of the servomotor the LED must be changed
         * @param on if true, will turn LED on, if false will turn off
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        virtual bool turnLED(uint8_t id, bool on) override;

        /**
         * @brief Turns the LED of the servomotor ON if it is currently OFF and OFF if it is currently ON
         * 
         * @param id the id of the servomotor the LED must be changed
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        virtual bool turnLED(uint8_t id) override;


        /**
         * @brief Changes speed of the specified servomotor
         * 
         * @param id the id of the servomotor whose speed has to change
         * @param newSpeed the value of the new speed
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        using AbstractController::changeSpeed;
        virtual bool changeSpeed(uint8_t id, uint16_t newSpeed) override;


        /**
         * @brief Sets the position of the servomotor
         * 
         * @param id the id of the servomotor whose position has to be changed
         * @param newPosition the new position of the servo
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        using AbstractController::setPosition;
        virtual bool setPosition(uint8_t id, uint16_t newPosition) override;


        /**
         * @brief Adds to the current target position
         * 
         * @param id the id of the servomotor whose position has to be changed
         * @param dx the number to add to the current goal position
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        using AbstractController::addPosition;
        virtual bool addPosition(uint8_t id, int dx) override;


        /**
         * @brief Enables or disables a servomotor's ability to move or hold a position (torque)
         * 
         * @param enable if true, enables the torque, otherwise, disables it
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        virtual bool enableTorque(int id, bool enable = true) override;

        /**
         * @brief Checks if torque is enabled for the given servomotor (see enableTorque() method)
         * 
         * @param id the id of the torque to check
         * @return true if enabled
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        virtual bool torqueEnabled(int id) override;


        /**
         * @brief Asks information from servomotor device and update the values in the class representing it
         * 
         * @param id the id of the servomotor to update
         * @return true if information has successfully been updated
         * @return false otherwise
         * 
         * Inherited method from AbstractController
         */
        using AbstractController::updateInfos;
        virtual bool updateInfos(uint8_t id) override;
    
};

    }
}

#endif
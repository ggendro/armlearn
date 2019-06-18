/**
 * @file abstractcontroller.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class AbstractController, used for controlling hardware or simulated device
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <thread>

#include "servomotor.h"
#include "iderror.h"
#include "connectionerror.h"
#include "outofrangeerror.h"


// Position of the servomotors to put the arm in backhoe position // TODO: to correct, to have generic positions
#define BACKHOE_POSITION {2048, 2048, 2048, 2048, 512, 256}
// Position of the servomotors to put the arm in sleep position
#define SLEEP_POSITION {2048, 1025, 1025, 1830, 512, 256}

// Time to wait for servomotors to reach each position (in milliseconds)
#define WAITING_TIME 5000
// Error margin between current and target position
#define POSITION_ERROR_MARGIN 40


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
 * @class AbstractController
 * @brief Provides an abstract interface to link servomotor classes with servomotor devices
 * Link with classes implemented
 * Link with devices (physical or simulated) to be implemented in inherited classes
 */
class AbstractController{

    protected:
        std::map<uint8_t, Servomotor*>* motors;

        DisplayMode mode;
        std::ostream& output;


    public:

        /**
         * @brief Construct a new AbstractController object
         * 
         * @param displayMode mode of display (see DisplayMode enum for more details)
         * @param out the output stream to display the results, standard std output by default
         */
        AbstractController(DisplayMode displayMode = except, std::ostream& out = std::cout);

        /**
         * @brief Destroys the Controller:: Controller object
         * 
         */
        virtual ~AbstractController();


        
        /**
         * @brief Connects the controller to the physical or simulated devices
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual void connect() = 0;


        /**
         * @brief Sends a ping to a device, the device will return its id, model number and firmware version
         * 
         * @param id the id of the device to send the ping to
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual void ping(uint8_t id) = 0;



        /**
         * @brief Gets the controller's mode of display (see DisplayMode enum for more details)
         * 
         * @return DisplayMode the current mode
         */
        DisplayMode getDisplayMode() const;

        /**
         * @brief Sets the controller's mode of display (see DisplayMode enum for more details)
         * 
         * @param newMode the new mode to set
         */
        void setDisplayMode(DisplayMode newMode);


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
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool changeId(uint8_t oldId, uint8_t newId) = 0;

        /**
         * @brief Turns the LED of the servomotor ON / OFF
         * 
         * @param id the id of the servomotor the LED must be changed
         * @param on if true, will turn LED on, if false will turn off
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool turnLED(uint8_t id, bool on) = 0;

        /**
         * @brief Turns the LED of the servomotor ON if it is currently OFF and OFF if it is currently ON
         * 
         * @param id the id of the servomotor the LED must be changed
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool turnLED(uint8_t id) = 0;


        /**
         * @brief Changes speed of the specified servomotor
         * 
         * @param id the id of the servomotor whose speed has to change
         * @param newSpeed the value of the new speed
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool changeSpeed(uint8_t id, uint16_t newSpeed) = 0;

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
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool setPosition(uint8_t id, uint16_t newPosition) = 0;

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
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool addPosition(uint8_t id, int dx) = 0;

        /**
         * @brief Adds to the goal position of all servos
         * 
         * @param dx the number to add to the servos' position
         */
        void addPosition(const std::vector<int>& dx);


        /**
         * @brief Get the real position of all servomotors at last update
         * 
         * @return std::vector<uint16_t> the positions of each servo
         */
        std::vector<uint16_t> getPosition() const;

        /**
         * @brief Checks whether the iven position is valid or not
         * 
         * @param position the position to verify
         * @return true if the position is valid
         * @return false otherwise
         */
        bool validPosition(std::vector<uint16_t> position) const;


        /**
         * @brief Enables or disables a servomotor's ability to move or hold a position (torque)
         * 
         * @param enable if true, enables the torque, otherwise, disables it
         * @return true if successfully changed
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool enableTorque(int id, bool enable = true) = 0;

        /**
         * @brief Checks if torque is enabled for the given servomotor (see enableTorque() method)
         * 
         * @param id the id of the torque to check
         * @return true if enabled
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool torqueEnabled(int id) = 0;


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
         * @brief Waits for device to reach its current target position
         * 
         * @param allowedTime the response time allowed, returns if no answer got before this time
         * @return true if a response occured within the allowed time
         * @return false otherwise  
         */
        bool waitFeedback(int allowedTime = WAITING_TIME);


        /**
         * @brief Asks information from servomotor device or simulation and update the values in the class representing it
         * 
         * @param id the id of the servomotor to update
         * @return true if information has successfully been updated
         * @return false otherwise
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual bool updateInfos(uint8_t id) = 0;

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


    friend class DeviceLearner;  
};

#endif
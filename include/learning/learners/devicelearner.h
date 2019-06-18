/**
 * @file devicelearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class DeviceLearner, used for computing positions from a controller
 * @version 0.1
 * @version 0.1
 * @date 2019-06-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DEVICELEARNER_H
#define DEVICELEARNER_H

#include <thread>

#include "abstractcontroller.h"
#include "learner.h"

// Min distance to the range limits of servomotors
#define RANGE_DISTANCE_SECURITY 10


/**
 * @class DeviceLearner
 * @brief Abstract class containing a learner that learns how to compute servomotor positions from coordinates and the servomotors state
 * 
 * Friend class of Servomotor and AbstractController classes
 */
class DeviceLearner : public Learner{

    protected:
        AbstractController* device;

        /**
         * @brief Returns the current state of each servomotor
         * 
         * @return std::vector<std::vector<uint16_t>> array containing the state of each servomotor as an array
         */
        std::vector<std::vector<uint16_t>> getDeviceState() const;

        /**
         * @brief Computes from a given position the closest position that is within the range ofeach servomotor, if all values are within ranges, returns the initial vector
         * 
         * @param position the positions to verify and modify if necessary
         * @return std::vector<uint16_t> array of servomotor positions
         * 
         * Template method, defined for uint16_t and double
         */
        template<class T> std::vector<T> getClosestValidPosition(std::vector<T> position, T securityThreshold = RANGE_DISTANCE_SECURITY) const;

    public:

        /**
         * @brief Constructs a new Device Learner object
         * 
         * @param controller the controller to link the learner to
         * @param testProp the proportion of test couples compared to learning couples
         */
        DeviceLearner(AbstractController* controller, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;
    

};


#endif
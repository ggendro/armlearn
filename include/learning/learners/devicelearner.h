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

#include "abstractcontroller.h"
#include "learner.h"

/**
 * @class DeviceLearner
 * @brief Abstract class containing a learner that learns how to compute servomotor positions from coordinates and the servomotors state
 * 
 * Friend class of Servomotor and AbstractController classes
 */
class DeviceLearner : public Learner{

    protected:
        AbstractController* device;

        std::vector<std::vector<uint16_t>> getDeviceState();

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
/**
 * @file setmodepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class SetModePyLearner, class executing learning on device using set mode
 * @version 0.1
 * @date 2019-08-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SETMODEPYLEARNER_H
#define SETMODEPYLEARNER_H

#include "bufferbasedpylearner.h"
#include "abstractcontroller.h"
#include "converter.h"

namespace armlearn {
    namespace learning {

/**
 * @class SetModePyLearner
 * @brief Class executing learning algorithm with set position mode for the device
 * 
 * Set mode returns the new device position in absolute
 */
class SetModePyLearner : public BufferBasedPyLearner{

    public:

        /**
         * @brief Constructs a new SetModePyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        SetModePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~SetModePyLearner();

        /**
         * @brief Formats the computation output for the device
         * 
         * 
         * @param output the output to format
         * @return std::vector<uint16_t> the formatted output
         * 
         * Implementation of SimplePyLearner method using set mode
         */
        virtual std::vector<uint16_t> formatOutput(const std::vector<double>& output) const override;


        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;

};


    }
}

#endif
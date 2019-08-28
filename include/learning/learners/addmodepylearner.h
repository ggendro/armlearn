/**
 * @file addmodepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class AddModePyLearner, class executing learning on device using add mode
 * @version 0.1
 * @date 2019-08-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ADDMODEPYLEARNER_H
#define ADDMODEPYLEARNER_H

#include "bufferbasedpylearner.h"
#include "abstractcontroller.h"
#include "converter.h"

namespace armlearn {
    namespace learning {

/**
 * @class AddModePyLearner
 * @brief Class executing learning algorithm with add position mode for the device
 * 
 * Add mode returns the new device position in relative, how much each servomotor should move
 */
class AddModePyLearner : public BufferBasedPyLearner{

    public:

        /**
         * @brief Constructs a new AddModePyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptsettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        AddModePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~AddModePyLearner();

        /**
         * @brief Formats the computation output for the device
         * 
         * 
         * @param output the output to format
         * @return std::vector<uint16_t> the formatted output
         * 
         * Implementation of SimplePyLearner method using add mode
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
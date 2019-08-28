/**
 * @file passivepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class PassivePyLearner, used for getting learned positions from computations via python module
 * @version 0.1
 * @date 2019-06-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef PASSIVEPYLEARNER_H
#define PASSIVEPYLEARNER_H

#include "simplepylearner.h"

namespace armlearn {
    namespace learning {

/**
 * @class PassivePyLearner
 * @brief Class linked to python module for simple positions computing and learning
 * 
 * The PassivePyLearner does not make any computations, it launches a python method for learning which will do all the computations
 * and answer requests from the module when interaction within the environment is required.
 * Contrary to the BufferBasedPyLearner class wich executes most of the algorithm by itself.
 */
class PassivePyLearner : public SimplePyLearner{

    public:

        /**
         * @brief Constructs a new PyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        PassivePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~PassivePyLearner();



        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         */
        virtual void learn() override;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         * Empty method
         */
        virtual void test() override;
        

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
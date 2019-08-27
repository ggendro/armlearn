/**
 * @file simplepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class BufferBasedPyLearner, used for computing positions via python module
 * @version 0.1
 * @date 2019-06-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef BUFFERBASEDPYLEARNER_H
#define BUFFERBASEDPYLEARNER_H

#include <algorithm>

#include "simplepylearner.h"
#include "outofrangeerror.h"


/**
 * @class BufferBasedPyLearner
 * @brief Class containing a buffer based algorithm linked to python module for simple positions computing and learning
 * 
 * Most of the learning algorithm is executed on C++ side. The computations are driven from this class, which asks
 * the python learner to execute the desired functions.
 * Contrary to the PassivePyLearner class wich does not make any computations on C++ side.
 */
class BufferBasedPyLearner : public SimplePyLearner{

    protected:

        /**
         * @class State
         * @brief Inner class storing a state of the learner (input, output and associated reward)
         * 
         */
        class State{

            private:
                std::vector<uint16_t>* input;
                std::vector<double>* output;
                std::vector<double>* reward;

            public:
                /**
                 * @brief Construct a new State object
                 * 
                 * @param inputVector the input of the state
                 * @param outputVector its corresponding output
                 * @param reward its corresponding reward
                 */
                State(std::vector<uint16_t>& inputVector, std::vector<double>& outputVector, std::vector<double>& reward);

                /**
                 * @brief Destroy the State object
                 * 
                 */
                ~State();

                /**
                 * @brief Get the Input
                 * 
                 * @return std::vector<uint16_t> the input
                 */
                std::vector<uint16_t> getInput() const;

                /**
                 * @brief Get the Output
                 * 
                 * @return std::vector<double> the output
                 */
                std::vector<double> getOutput() const;

                /**
                 * @brief Get the Reward
                 * 
                 * @return double the reward
                 */
                std::vector<double> getReward() const;

                /**
                 * @brief Checks if the input of the state is equal to the one in parameter
                 * 
                 * @param s the state to compare to
                 * @return true if the two states are equal
                 * @return false otherwise
                 */
                bool hasSameInput(State& s) const;
        };


    public:

        /**
         * @brief Constructs a new BufferBasedPyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        BufferBasedPyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the BufferBasedPyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~BufferBasedPyLearner();



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





#endif
/**
 * @file simplepylearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class SimplePyLearner, used for executing simple computation to determine positions, using python script
 * @version 0.1
 * @date 2019-06-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SIMPLEPYLEARNER_H
#define SIMPLEPYLEARNER_H

#include <algorithm>

#include "pylearner.h"
#include "converter.h"
#include "outofrangeerror.h"


// Number of iterations of the learning
#define LEARN_NB_ITERATIONS 50000
// Number of movents allowed to reach target during learning
#define LEARN_NB_MOVEMENTS 150
// Margin of error allowing to stop learning if error is below the number
#define LEARN_ERROR_MARGIN 0.05
// Number of invalid movements allowed during one iteration
#define MAX_NULL_MOVE 10

// Coefficient of target error (difference between the real output and the target output, to minimize) when computing error between input and output
#define TARGET_COEFF 0.014
// Coefficient of movement error (distance browsed by servomotors to reach target, to minimize) when computing error between input and output
#define MOVEMENT_COEFF 0.0024
// Coefficient of valid position error (returned if position is not valid)
#define VALID_COEFF -30
// Coefficient decreasing the reward for a state as the state is closer to the initial state
#define DECREASING_REWARD 0.99

/**
 * @class SimplePyLearner
 * @brief Class containing a simple learner that learns how to compute servomotor positions from coordinates without obstacles or disabled servomotors, via python script
 * 
 */
class SimplePyLearner : public PyLearner{

    protected:
        Converter* verifier;

        /**
         * @brief Computes resulting reward of the output for the corresponding input and servomotor states
         * 
         * @param input target to reach
         * @param output servomotor positions computed, to evaluate
         * @return double output reward
         * 
         * Takes into account for the computation:
         *  - the distance between the target coordinates and the coordinates resulting from the output positions, the greater it is the greater the error will be
         *  - the distance browsed by servomotors, the greater it is the greater the error will be
         *  - if the distance to the target increased or decreased after output applied
         * 
         * Template method, defined for uint16_t and double
         */
        template<class R, class T> double computeReward(const std::vector<R> input, const std::vector<T> output) const;

        /**
         * @class State
         * @brief Inner class storing a state of the learner (input, output and associated reward)
         * 
         */
        class State{

            private:
                std::vector<uint16_t>* input;
                std::vector<int>* output;
                std::vector<double>* reward;

            public:
                /**
                 * @brief Construct a new State object
                 * 
                 * @param inputVector the input of the state
                 * @param outputVector its corresponding output
                 * @param reward its corresponding reward
                 */
                State(std::vector<uint16_t>& inputVector, std::vector<int>& outputVector, std::vector<double>& reward);

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
                 * @return std::vector<int> the output
                 */
                std::vector<int> getOutput() const;

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
         * @brief Constructs a new PyLearner object
         * 
         * @param controller the controller to link the learner to
         * @param learningScriptSettings the name of the json file containing the required settings of the python modules to use  
         * @param testProp the proportion of test couples compared to learning couples
         */
        SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~SimplePyLearner();



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
         * @brief Computes an output from an input (to use after learning and validation with testing)
         * 
         * @param input
         * @return Output
         */
        virtual Output* produce(const Input& input) override;

        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const override;

};





#endif
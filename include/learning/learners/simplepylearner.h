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

#include "pylearner.h"
#include "converter.h"

// Coefficient of target error (difference between the real output and the target output, to minimize)
#define TARGET_COEFF 1
// Coefficient of movement error (distance browsed by servomotors to reach target, to minimize)
#define MOVEMENT_COEFF 0.4


/**
 * @class SimplePyLearner
 * @brief Class containing a simple learner that learns how to compute servomotor positions from coordinates without obstacles or disabled servomotors, via python script
 * 
 */
class SimplePyLearner : public PyLearner{

    protected:
        Converter* verifier;

        /**
         * @brief Intermediary method used in computeExepectedOutput, computes resulting error of the output for the corresponding input and servomotor states
         * 
         * @param input target to reach
         * @param output servomotor positions computed, to evaluate
         * @return double output error
         * 
         * Takes into account for the computation:
         *  - the distance between the target coordinates and the coordinates resulting from the output positions, the greater it is the greater the error will be
         *  - the distance browsed by servomotors, the greater it is the greater the error will be
         */
        template<class R, class T> double intermediaryComputeError(const std::vector<R> input, const std::vector<T> output){
            return TARGET_COEFF * computeSquaredError(input, verifier->computeServoToCoord(std::vector<uint16_t>(output.begin(), output.end()))->getCoord()) + MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
        }

        /**
         * @brief Computes error between learning output and expected output based on the input
         * 
         * @param input the input coordinates
         * @param output the resulting output from learner
         * @return std::vector<double> error between output given and expected output
         */
        std::vector<double> computeExpectedOutput(const std::vector<uint16_t> input, const std::vector<uint16_t> output);


    public:

        /**
         * @brief Constructs a new PyLearner object
         * 
         */
        SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScript = PY_LEARN_FILE, double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the PyLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~SimplePyLearner();



        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         * Empty method
         */
        virtual void learn() override;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         * Empty method
         */
        virtual void test() override;

         /**
         * @brief Computes an output from an input (to use afetr learning and validation with testing)
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
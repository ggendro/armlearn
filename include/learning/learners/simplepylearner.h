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


// Number of iterations of the learning
#define LEARN_NB_ITERATIONS 2000
// Margin of error allowing to stop learning if error is below the number
#define LEARN_ERROR_MARGIN 0.05
// Size of the output grid, used for discretization of the output space
#define GRANULARITY_GRID 50

// Coefficient of target error (difference between the real output and the target output, to minimize) when computing error between input and output
#define TARGET_COEFF 0.1
// Coefficient of movement error (distance browsed by servomotors to reach target, to minimize) when computing error between input and output
#define MOVEMENT_COEFF 0.04
// Coefficient of valid position error (returned if position is not valid)
#define VALID_COEFF 500


/**
 * @class SimplePyLearner
 * @brief Class containing a simple learner that learns how to compute servomotor positions from coordinates without obstacles or disabled servomotors, via python script
 * 
 */
class SimplePyLearner : public PyLearner{

    protected:
        Converter* verifier;

        /**
         * @brief Computes resulting error of the output for the corresponding input and servomotor states
         * 
         * @param input target to reach
         * @param output servomotor positions computed, to evaluate
         * @return double output error
         * 
         * Takes into account for the computation:
         *  - the distance between the target coordinates and the coordinates resulting from the output positions, the greater it is the greater the error will be
         *  - the distance browsed by servomotors, the greater it is the greater the error will be
         * 
         * Template method, defined for uint16_t and double
         */
        template<class R, class T> double computeError(const std::vector<R> input, const std::vector<T> output) const;


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
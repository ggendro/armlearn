/**
 * @file simplelearner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class SimpleLearner, used for computing positions using AI methods
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SIMPLELEARNER_H
#define SIMPLELEARNER_H



#include "learner.h"
#include "simpleinput.h"
#include "simpleoutput.h"
#include "optimcartesianconverter.h"


/**
 * @class SimpleLearner
 * @brief Class containing a simple learner that learns how to compute servomotor positions from coordinates without obstacles or disabled servomotors
 * 
 */
class SimpleLearner : public Learner{

    protected:
        Converter* verifier;


    public:

        /**
         * @brief Constructs a new SimpleLearner object
         * 
         */
        SimpleLearner(double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the SimpleLearner object
         * 
         * Destroys all the registered inputs and outputs
         */
        virtual ~SimpleLearner();



        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         * Abstract method
         */
        virtual void learn() override;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         * Abstract method
         */
        virtual void test() override;

        /**
         * @brief Computes an output from an input (to use afetr learning and validation with testing)
         * 
         * @param input
         * @return Output
         */
        virtual Output& produce(const Input& input) override;

};





#endif
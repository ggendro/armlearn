/**
 * @file learner.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class Learner, used for computing positions using AI methods
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef LEARNER_H
#define LEARNER_H

#include <iostream>
#include <vector>
#include <map>
#include <math.h>

#include "input.h"
#include "output.h"


#define DEFAULT_TEST_PROPORTION 0.33
#define ERROR_MARGIN_PROPORTION 0.025


/**
 * @class Learner
 * @brief Abstract class containing a learner that learns how to compute servomotor positions from coordinates
 * 
 */
class Learner{ // TODO: Adapt for obstacles, disabled servo...

    protected:
        std::map<Input*, Output*>* learningSet;
        std::map<Input*, Output*>* testingSet;

        std::map<Input*, Output*>* sets[2];

        double testProportion;


        /**
         * @brief Template method, computes the sum of squared errors (SSE) between the two vectors
         * 
         * @tparam R type of the values contained in the first vector, has to be countable and automatically converrtible with the second type
         * @tparam T type of the values contained in the second vector, has to be countable and automatically converrtible with the first type
         * @param target values of the first vector
         * @param real values of the second vector
         * @return double the computed SSE
         * 
         * Template method, defined for uint16_t and double
         */
        template<class R, class T> double computeSquaredError(const std::vector<R> target, const std::vector<T> real) const;


        /**
         * @brief Adds an input/output couple to the specified set
         * 
         * @param input of the couple
         * @param output of the couple
         * @param test if true, adds to the testing set, otherwise to the learning set
         */
        void addToSet(Input* input, Output* output, bool test = false);

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         * @param test if true, removes from the testing set, otherwise from the learning set 
         */
        void removeFromSet(const Input& input, bool test = false);
        
        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         * @param test if true, removes from the testing set, otherwise from the learning set 
         */
        void removeFromSet(const Input* input, bool test = false);


    public:

        /**
         * @brief Constructs a new Learner object
         * 
         * @param testProp the proportion of test couples compared to learning couples
         */
        Learner(double testProp = DEFAULT_TEST_PROPORTION);

        /**
         * @brief Destroys the Learner object
         * 
         * Destroys all couples in the learner
         */
        virtual ~Learner();


        /**
         * @brief Sets the Test Proportion
         * 
         * @param newprop the new proportion to set
         */
        void setTestProportion(double newprop);

        /**
         * @brief Gets the Test Proportion
         * 
         * @return double the proportion
         */
        double getTestProportion() const;

        /**
         * @brief Gets the size of the learning set
         * 
         * @return int the size
         */
        int getLearningSize() const;

        /**
         * @brief Gets the size of the testing set
         * 
         * @return int the size
         */
        int getTestingSize() const;

        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const;


        /**
         * @brief Adds an input/output couple to the learning set
         * 
         * @param input of the learning couple
         * @param output of the learning couple
         */
        void addToLearningSet(Input* input, Output* output);

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         */
        void removeFromLearningSet(const Input& input);

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         */
        void removeFromLearningSet(const Input* input);


        /**
         * @brief Adds an input/output couple to the testing set
         * 
         * @param input of the learning couple
         * @param output of the learning couple
         */
        void addToTestingSet(Input* input, Output* output);

        /**
         * @brief Removes a couple from the testing set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         */
        void removeFromTestingSet(const Input& input);

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         */
        void removeFromTestingSet(const Input* input);


        /**
         * @brief Generates a random testing set from the learning set with proportion testProportion
         * 
         * Consumes random couples from the learning set to add them to the testing set until the proportion is reached
         * If proportion is already above the given one, removes couples randomly
         * 
         */
        void generateTestingSet();



        /**
         * @brief Executes a learning algorithm on the learning set
         * 
         * Abstract method
         */
        virtual void learn() = 0;

        /**
         * @brief Tests the efficiency of the learning 
         * 
         * Abstract method
         */
        virtual void test() = 0;

        /**
         * @brief Computes an output from an input (to use afetr learning and validation with testing)
         * 
         * @param input
         * @return Output 
         * 
         * Abstract method
         */
        virtual Output* produce(const Input& input) = 0;

};





#endif
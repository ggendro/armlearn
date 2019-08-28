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

namespace armlearn {
    namespace learning {


#define DEFAULT_TEST_PROPORTION 0.33
#define ERROR_MARGIN_PROPORTION 0.025


/**
 * @class Learner
 * @brief Abstract class containing a learner that learns how to compute servomotor positions from coordinates
 * 
 */
template<class U, class V> class Learner{ // TODO: Adapt for obstacles, disabled servo...

    protected:
        std::map<Input<U>*, Output<V>*>* learningSet;
        std::map<Input<U>*, Output<V>*>* testingSet;

        std::map<Input<U>*, Output<V>*>* sets[2];

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
        template<class R, class T> double computeSquaredError(const std::vector<R> target, const std::vector<T> real) const{
            double sse = 0;

            auto ptrR = real.cbegin();
            for(auto ptrT=target.cbegin(); ptrT != target.cend(); ptrT++){
                sse += std::pow(*ptrT - *ptrR, 2);
                ptrR++;
            }

            return std::sqrt(sse);
        }


        /**
         * @brief Adds an input/output couple to the specified set
         * 
         * @param input of the couple
         * @param output of the couple
         * @param test if true, adds to the testing set, otherwise to the learning set
         */
        void addToSet(Input<U>* input, Output<V>* output, bool test = false){
            sets[test]->insert(std::pair<Input<U>*, Output<V>*>(input, output));
        }

        /**
         * @brief Removes a couple from a set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         * @param test if true, removes from the testing set, otherwise from the learning set 
         */
        void removeFromSet(const Input<U>& input, bool test = false){
            for(auto ptr = sets[test]->begin(); ptr != sets[test]->end(); ptr++){
                if((*ptr->first) == input){
                    sets[test]->erase(ptr);
                    return;
                }
            }
        }
        
        /**
         * @brief Removes a couple from a set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         * @param test if true, removes from the testing set, otherwise from the learning set 
         */
        void removeFromSet(const Input<U>* input, bool test = false){
            for(auto ptr = sets[test]->begin(); ptr != sets[test]->end(); ptr++){
                if(ptr->first == input){
                    sets[test]->erase(ptr);
                    return;
                }
            }
        }


    public:

        /**
         * @brief Constructs a new Learner object
         * 
         * @param testProp the proportion of test couples compared to learning couples
         */
        Learner(double testProp = DEFAULT_TEST_PROPORTION):testProportion(testProp){
            learningSet = new std::map<Input<U>*, Output<V>*>();
            testingSet = new std::map<Input<U>*, Output<V>*>();

            sets[0] = learningSet;
            sets[1] = testingSet;

            std::srand (time(NULL));
        }

        /**
         * @brief Destroys the Learner object
         * 
         * Destroys all couples in the learner
         */
        virtual ~Learner(){
            for(int i = 0; i < 2; i++){
                for(auto ptr = sets[i]->begin(); ptr != sets[i]->end(); ptr++){
                    delete ptr->first;
                    delete ptr->second;
                }

                delete sets[i];
            }
        }


        /**
         * @brief Sets the Test Proportion
         * 
         * @param newprop the new proportion to set
         */
        void setTestProportion(double newProp){
            testProportion = newProp;
        }

        /**
         * @brief Gets the Test Proportion
         * 
         * @return double the proportion
         */
        double getTestProportion() const{
            return testProportion;
        }

        /**
         * @brief Gets the size of the learning set
         * 
         * @return int the size
         */
        int getLearningSize() const{
            return learningSet->size();
        }

        /**
         * @brief Gets the size of the testing set
         * 
         * @return int the size
         */
        int getTestingSize() const{
            return testingSet->size();
        }

        /**
         * @brief Returns the state of the learner under string format
         * 
         * @return std::string the state of the learner
         */
        virtual std::string toString() const{
            std::stringstream res;

            res << "Learner" << std::endl;

            res << "Training set : " << "(ratio : " << (1 - testProportion) << ")" << std::endl;
            for(auto ptr = learningSet->begin(); ptr != learningSet->end(); ptr++) res << ptr->first->toString() << " --> " << ptr->second->toString() << std::endl;

            res << "Test set : " << "(ratio : " << testProportion << ")" << std::endl;
            for(auto ptr = testingSet->begin(); ptr != testingSet->end(); ptr++) res << ptr->first->toString() << " --> " << ptr->second->toString() << std::endl;

            return res.str();
        }


        /**
         * @brief Adds an input/output couple to the learning set
         * 
         * @param input of the learning couple
         * @param output of the learning couple
         */
        void addToLearningSet(Input<U>* input, Output<V>* output){
            addToSet(input, output, false);
        }

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         */
        void removeFromLearningSet(const Input<U>& input){
            removeFromSet(input, false);
        }

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         */
        void removeFromLearningSet(const Input<U>* input){
            removeFromSet(input, false);
        }


        /**
         * @brief Adds an input/output couple to the testing set
         * 
         * @param input of the learning couple
         * @param output of the learning couple
         */
        void addToTestingSet(Input<U>* input, Output<V>* output){
            addToSet(input, output, true);
        }

        /**
         * @brief Removes a couple from the testing set
         * 
         * @param input of the couple to remove, Input object with the same attributes as the one wanted to be removed
         */
        void removeFromTestingSet(const Input<U>& input){
            removeFromSet(input, true);
        }

        /**
         * @brief Removes a couple from the learning set
         * 
         * @param input of the couple to remove, pointer to the input to remove
         */
        void removeFromTestingSet(const Input<U>* input){
            removeFromSet(input, true);
        }


        /**
         * @brief Generates a random testing set from the learning set with proportion testProportion
         * 
         * Consumes random couples from the learning set to add them to the testing set until the proportion is reached
         * If proportion is already above the given one, removes couples randomly
         * 
         */
        void generateTestingSet(){
            if(((double) testingSet->size()) / (learningSet->size()) > testProportion + ERROR_MARGIN_PROPORTION){

                while(((double) testingSet->size()) / (learningSet->size()) > testProportion){ // If too many couples in testing set, removes them until proportion is reached
                    auto ptr = testingSet->begin();
                    std::advance(ptr, std::rand() % testingSet->size());

                    testingSet->erase(ptr);
                }

            }else{

                while(((double) testingSet->size()) / (learningSet->size()) < testProportion - ERROR_MARGIN_PROPORTION){ // Otherwise, moves couples from learning set to testing set
                    auto ptr = learningSet->begin();
                    std::advance(ptr, std::rand() % learningSet->size());

                    testingSet->insert(std::move(*ptr));
                    learningSet->erase(ptr);
                }

            }
        }



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
        virtual Output<V>* produce(const Input<U>& input) = 0;

};

    }
}


#endif
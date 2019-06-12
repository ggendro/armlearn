/**
 * @copyright Copyright (c) 2019
 */


#include "learner.h"


Learner::Learner(double testProp):testProportion(testProp){
    learningSet = new std::map<Input*, Output*>();
    testingSet = new std::map<Input*, Output*>();

    sets[0] = learningSet;
    sets[1] = testingSet;

    std::srand (time(NULL));
}

Learner::~Learner(){
    for(int i = 0; i < 2; i++){
        for(auto ptr = sets[i]->begin(); ptr != sets[i]->end(); ptr++){
            delete ptr->first;
            delete ptr->second;
        }

        delete sets[i];
    }
}


void Learner::setTestProportion(double newProp){
    testProportion = newProp;
}

double Learner::getTestProportion() const{
    return testProportion;
}

int Learner::getLearningSize() const{
    return learningSet->size();
}

int Learner::getTestingSize() const{
    return testingSet->size();
}


std::string Learner::toString() const{
    std::stringstream res;

    res << "Learner" << std::endl;

    res << "Training set : " << "(ratio : " << (1 - testProportion) << ")" << std::endl;
    for(auto ptr = learningSet->begin(); ptr != learningSet->end(); ptr++) res << ptr->first->toString() << " --> " << ptr->second->toString() << std::endl;

    res << "Test set : " << "(ratio : " << testProportion << ")" << std::endl;
    for(auto ptr = testingSet->begin(); ptr != testingSet->end(); ptr++) res << ptr->first->toString() << " --> " << ptr->second->toString() << std::endl;

    return res.str();
}



void Learner::addToSet(Input* input, Output* output, bool test){
    sets[test]->insert(std::pair<Input*, Output*>(input, output));
}

void Learner::removeFromSet(const Input& input, bool test){
    for(auto ptr = sets[test]->begin(); ptr != sets[test]->end(); ptr++){
        if((*ptr->first) == input){
            sets[test]->erase(ptr);
            return;
        }
    }
}

void Learner::removeFromSet(const Input* input, bool test){
    for(auto ptr = sets[test]->begin(); ptr != sets[test]->end(); ptr++){
        if(ptr->first == input){
            sets[test]->erase(ptr);
            return;
        }
    }
}



void Learner::addToLearningSet(Input* input, Output* output){
    addToSet(input, output, false);
}

void Learner::removeFromLearningSet(const Input& input){
    removeFromSet(input, false);
}

void Learner::removeFromLearningSet(const Input* input){
    removeFromSet(input, false);
}


void Learner::addToTestingSet(Input* input, Output* output){
    addToSet(input, output, true);
}

void Learner::removeFromTestingSet(const Input& input){
    removeFromSet(input, true);
}

void Learner::removeFromTestingSet(const Input* input){
    removeFromSet(input, true);
}


void Learner::generateTestingSet(){
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
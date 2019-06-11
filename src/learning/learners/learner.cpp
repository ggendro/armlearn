/**
 * @copyright Copyright (c) 2019
 */


#include "learner.h"


Learner::Learner(double testProp):testProportion(testProp){
    learningSet = new std::map<Input, Output>();
    testingSet = new std::map<Input, Output>();

    sets[0] = learningSet;
    sets[1] = testingSet;

    std::srand (time(NULL));
}

Learner::~Learner(){
    for(int i = 0; i < 2; i++){
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


void Learner::addToSet(const Input& input, const Output& output, bool test){
    sets[test]->insert(std::pair<Input, Output>(input, output));
}

void Learner::removeFromSet(const Input& input, bool test){
    sets[test]->erase(input);
}


void Learner::addToLearningSet(const Input& input, const Output& output){
    addToSet(input, output, false);
}

void Learner::removefromLearningSet(const Input& input){
    removeFromSet(input, false);
}


void Learner::addToTestingSet(const Input& input, const Output& output){
    addToSet(input, output, true);
}

void Learner::removefromTestingSet(const Input& input){
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
/**
 * @copyright Copyright (c) 2019
 */


#include "simplelearner.h"


SimpleLearner::SimpleLearner(AbstractController* controller, Converter* converter, double testProp):Learner(testProp){
    verifier = converter;
    device = controller;
}

SimpleLearner::~SimpleLearner(){
    
}


void SimpleLearner::learn(){

}

void SimpleLearner::test(){

}

Output SimpleLearner::produce(const Input& input){
    SimpleOutput res({1, 3, 5});
    return res;
}
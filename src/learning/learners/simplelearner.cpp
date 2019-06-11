/**
 * @copyright Copyright (c) 2019
 */


#include "simplelearner.h"


SimpleLearner::SimpleLearner(double testProp):Learner(testProp){
    verifier = new OptimCartesianConverter();

    // TODO: build converter object
}

SimpleLearner::~SimpleLearner(){
    delete verifier;
}


void SimpleLearner::learn(){ // TODO: to do...

}

void SimpleLearner::test(){

}

Output& SimpleLearner::produce(const Input& input){
    Output res;
    return res;
}
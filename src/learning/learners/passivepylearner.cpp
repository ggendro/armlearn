/**
 * @copyright Copyright (c) 2019
 */


#include "passivepylearner.h"


PassivePyLearner::PassivePyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings, double testProp):SimplePyLearner(controller, converter, learningScriptSettings, testProp){

}

PassivePyLearner::~PassivePyLearner(){
    
}


void PassivePyLearner::learn(){

}

void PassivePyLearner::test(){

}


std::string PassivePyLearner::toString() const{
    std::stringstream rep;
    rep << "Passive " << SimplePyLearner::toString();

    return rep.str();
}
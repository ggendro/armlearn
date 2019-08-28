/**
 * @copyright Copyright (c) 2019
 */


#include "passivepylearner.h"

using namespace armlearn;
using namespace learning;


PassivePyLearner::PassivePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings, double testProp):SimplePyLearner(controller, converter, learningScriptSettings, testProp){

}

PassivePyLearner::~PassivePyLearner(){
    
}


void PassivePyLearner::learn(){
    // TODO: to do
}

void PassivePyLearner::test(){
    // TODO: to do
}


std::string PassivePyLearner::toString() const{
    std::stringstream rep;
    rep << "Passive " << SimplePyLearner::toString();

    return rep.str();
}
/**
 * @copyright Copyright (c) 2019
 */

#include "setmodepylearner.h"

SetModePyLearner::SetModePyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings, double testProp):BufferBasedPyLearner(controller, converter, learningScriptSettings, testProp){
    
}

SetModePyLearner::~SetModePyLearner(){
    
}


std::vector<uint16_t> SetModePyLearner::formatOutput(const std::vector<double>& output) const{
    return device->toValidPosition(device->scalePosition(output, -M_PI, M_PI));
}

std::string SetModePyLearner::toString() const{
    std::stringstream rep;
    rep << "SetMode " << BufferBasedPyLearner::toString();

    return rep.str();
}
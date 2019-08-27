/**
 * @copyright Copyright (c) 2019
 */

#include "addmodepylearner.h"

AddModePyLearner::AddModePyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings, double testProp):BufferBasedPyLearner(controller, converter, learningScriptSettings, testProp){
    
}

AddModePyLearner::~AddModePyLearner(){
    
}


std::vector<uint16_t> AddModePyLearner::formatOutput(const std::vector<double>& output) const{
        auto incr = device->scalePosition(output, 0, 2*M_PI);
        auto decr = device->scalePosition(output, -2*M_PI, 0);
        auto currentPos = device->getPosition();

        std::vector<uint16_t> res;
        for(int i=0; i < incr.size(); i++) res.push_back(std::max(0, currentPos[i] + (output[i] > 0 ? incr[i] : -decr[i])));

        return device->toValidPosition(res);
}

std::string AddModePyLearner::toString() const{
    std::stringstream rep;
    rep << "Add-Mode " << BufferBasedPyLearner::toString();

    return rep.str();
}
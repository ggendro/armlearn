/**
 * @copyright Copyright (c) 2019
 */

#include "smalladdmodepylearner.h"

using namespace armlearn;
using namespace learning;

SmallAddModePyLearner::SmallAddModePyLearner(communication::AbstractController* controller, kinematics::Converter* converter, std::string learningScriptSettings, double testProp):BufferBasedPyLearner(controller, converter, learningScriptSettings, testProp){
    
}

SmallAddModePyLearner::~SmallAddModePyLearner(){
    
}


std::vector<uint16_t> SmallAddModePyLearner::formatOutput(const std::vector<double>& output) const{
        auto currentPos = device->getPosition();

        std::vector<uint16_t> res;
        auto outPtr = output.cbegin();
        for(auto posPtr=currentPos.cbegin(); posPtr < currentPos.cend() && outPtr < output.cend(); posPtr++){
            if(*outPtr > 1) res.push_back(*posPtr + 1);
            else if(*outPtr < -1) res.push_back(std::max(0, *posPtr - 1));
            else res.push_back(*posPtr);

            outPtr++;
        }

        return device->toValidPosition(res);
}

std::string SmallAddModePyLearner::toString() const{
    std::stringstream rep;
    rep << "SmallAdd-Mode " << BufferBasedPyLearner::toString();

    return rep.str();
}
/**
 * @copyright Copyright (c) 2019
 */


#include "simplepylearner.h"


SimplePyLearner::SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings, double testProp):PyLearner(controller, learningScriptSettings, testProp){
    verifier = converter;
}

SimplePyLearner::~SimplePyLearner(){
    
}


template<class R, class T> double SimplePyLearner::computeReward(const std::vector<R> input, const std::vector<T> output) const{
    std::vector<uint16_t> positionOutput;
    auto position = device->getPosition();

    auto outPtr = output.cbegin();
    for(auto posPtr = position.cbegin(); posPtr < position.cend(); posPtr++) {
        positionOutput.push_back((uint16_t)(*posPtr + *outPtr));
        outPtr++;
    }

    if(!device->validPosition(positionOutput)) return VALID_COEFF;
    
    //auto oldCoords = verifier->computeServoToCoord(position)->getCoord();
    auto newCoords = verifier->computeServoToCoord(positionOutput)->getCoord();


    //return TARGET_COEFF * (computeSquaredError(input, oldCoords) - computeSquaredError(input, newCoords)) - MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
    //*
    auto err = computeSquaredError(input, newCoords);
    if(abs(err) < LEARN_ERROR_MARGIN) return -VALID_COEFF;
    
    return -TARGET_COEFF * err - MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);  
    //*/
}
template double SimplePyLearner::computeReward<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::computeReward<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::computeReward<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<uint16_t, int>(const std::vector<uint16_t> input, const std::vector<int> output) const;
template double SimplePyLearner::computeReward<int, uint16_t>(const std::vector<int> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<int, int>(const std::vector<int> input, const std::vector<int> output) const;



Output* SimplePyLearner::produce(const Input& input){
    std::vector<uint16_t> fullInput(input.getInput()); // Create input of the DNN, add the target coordinates
    auto state = DeviceLearner::getDeviceState(); // Get state of servomotors

    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
    }

    return new Output(pyCompute(fullInput));
}


std::string SimplePyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << PyLearner::toString();

    return rep.str();
}
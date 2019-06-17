/**
 * @copyright Copyright (c) 2019
 */


#include "simplepylearner.h"


SimplePyLearner::SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScript, double testProp):PyLearner(controller, learningScript, testProp){
    verifier = converter;
}

SimplePyLearner::~SimplePyLearner(){
    
}


template<class R, class T> double SimplePyLearner::computeError(const std::vector<R> input, const std::vector<T> output) const{
    auto v = verifier->computeServoToCoord(std::vector<uint16_t>(output.begin(), output.end()))->getCoord();
    std::cout << "Coordinates : "; for(auto val : v) std::cout << val << " "; std::cout << " instead of "; for(auto val : input) std::cout << val << " "; std::cout << std::endl;
    return TARGET_COEFF * computeSquaredError(input, verifier->computeServoToCoord(std::vector<uint16_t>(output.begin(), output.end()))->getCoord()) + MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
}
template double SimplePyLearner::computeError<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeError<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;



void SimplePyLearner::learn(){

}

void SimplePyLearner::test(){

}


Output* SimplePyLearner::produce(const Input& input){
    std::vector<uint16_t> fullInput(input.getInput()); // Create input of the DNN, add the target coordinates

    auto state = DeviceLearner::getDeviceState(); // get state of servomotors

    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
    }

    std::vector<uint16_t> output;
    double error = 0;

    for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){
        std::cout << "Iteration " << nbIt << " : " << std::endl;
        output = pyLearn(fullInput, {error}); // Computation of output
        error = computeError(input.getInput(), output); // Computation of expected output
        std::cout << "Error : " << error << std::endl;

        if(error < LEARN_ERROR_MARGIN) break; // Stop learning if error is within threshold
    }

    return new Output(output);
}


std::string SimplePyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << PyLearner::toString();

    return rep.str();
}
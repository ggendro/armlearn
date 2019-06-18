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
    std::vector<uint16_t> uintOutput(output.cbegin(), output.cend());
    std::transform(uintOutput.begin(), uintOutput.end(), device->getPosition().begin(), uintOutput.begin(), std::plus<uint16_t>());

    if(!device->validPosition(uintOutput)) return VALID_COEFF;
    auto coords = verifier->computeServoToCoord(uintOutput)->getCoord();

    std::cout << "Coordinates : "; for(auto val : coords) std::cout << val << " "; std::cout << " instead of "; for(auto val : input) std::cout << val << " "; std::cout << std::endl;
    return TARGET_COEFF * computeSquaredError(input, coords) + MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
}
template double SimplePyLearner::computeError<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeError<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;


void SimplePyLearner::learn(){

    for(auto lsetPtr = learningSet->cbegin(); lsetPtr != learningSet->cend(); lsetPtr++){
        device->goToBackhoe(); // Reset position
        device->waitFeedback();

        std::vector<int> output;
        double error = 0;

        for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){
            std::cout << "Iteration " << nbIt << " : " << std::endl;


            std::vector<uint16_t> fullInput(lsetPtr->first->getInput()); // Create input of the DNN, add the target coordinates
            auto state = DeviceLearner::getDeviceState(); // Get state of servomotors

            for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
                fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
            }
            
            output = pyLearn(fullInput, {error}); // Computation of output
            error = computeError(lsetPtr->first->getInput(), output); // Computation of expected output
            std::cout << "Error : " << error << std::endl;

            if(error < VALID_COEFF){ // If position is valid (within range)
                device->addPosition(output); // Update position
                device->waitFeedback();
            }else{
                std::cout << "Error too important : movement not allowed" << std::endl;
            }

            if(error < LEARN_ERROR_MARGIN) break; // Stop learning if error is within threshold
        }

    }

}

void SimplePyLearner::test(){

}


Output* SimplePyLearner::produce(const Input& input){
    std::vector<uint16_t> fullInput(input.getInput()); // Create input of the DNN, add the target coordinates
    auto state = DeviceLearner::getDeviceState(); // Get state of servomotors

    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
    }

    return new Output(pyLearn(fullInput));
}


std::string SimplePyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << PyLearner::toString();

    return rep.str();
}
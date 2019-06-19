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
    std::vector<uint16_t> positionOutput;
    auto pos= device->getPosition();
    auto outPtr = output.cbegin();
    for(auto posPtr = pos.cbegin(); posPtr < pos.cend(); posPtr++) {
        positionOutput.push_back((uint16_t)(*posPtr + *outPtr));
        outPtr++;
    }

    std::cout << "Resulting position : "; for(auto v : positionOutput) std::cout << v << " "; std::cout << std::endl;
    if(!device->validPosition(positionOutput)) return VALID_COEFF;
    auto coords = verifier->computeServoToCoord(positionOutput)->getCoord();

    std::cout << "Coordinates : "; for(auto val : coords) std::cout << val << " "; std::cout << " instead of "; for(auto val : input) std::cout << val << " "; std::cout << std::endl;
    return TARGET_COEFF * computeSquaredError(input, coords) + MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
}
template double SimplePyLearner::computeError<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeError<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::computeError<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeError<uint16_t, int>(const std::vector<uint16_t> input, const std::vector<int> output) const;
template double SimplePyLearner::computeError<int, uint16_t>(const std::vector<int> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeError<int, int>(const std::vector<int> input, const std::vector<int> output) const;


void SimplePyLearner::learn(){

    for(auto lsetPtr = learningSet->cbegin(); lsetPtr != learningSet->cend(); lsetPtr++){

        for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){
            std::cout << "Reset device position...";
            device->goToBackhoe(); // Reset position
            device->waitFeedback();

            bool stop = false;

            for(int nbMove = 0; nbMove < LEARN_NB_MOVEMENTS; nbMove++){
                std::cout << "Iteration " << nbIt << " - Move " << nbMove << " : " << std::endl;


                std::vector<uint16_t> fullInput(lsetPtr->first->getInput()); // Create input of the DNN, add the target coordinates
                auto state = DeviceLearner::getDeviceState(); // Get state of servomotors

                for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
                    fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
                }
                
                std::cout << "Computing output..." << std::endl;
                std::vector<int> output = pyCompute(fullInput); // Computation of output
                double error = computeError(lsetPtr->first->getInput(), output); // Computation of error
                std::cout << "Error : " << error << std::endl;

                std::cout << "Backtracking error to learner..." << std::endl;
                pyLearn(fullInput, {error}); // Return error to learner

                if(error < VALID_COEFF){ // If position is valid (within range)
                    device->addPosition(output); // Update position
                    device->waitFeedback();

                    std::cout << "Updating position..." << std::endl;
                }else{
                    std::cout << "Error too important : movement not allowed" << std::endl;
                }

                if(error < LEARN_ERROR_MARGIN){ // Stop learning if error is within threshold
                    stop = true;
                    break;
                }
            }
            
            if(stop) break;
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

    return new Output(pyCompute(fullInput));
}


std::string SimplePyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << PyLearner::toString();

    return rep.str();
}
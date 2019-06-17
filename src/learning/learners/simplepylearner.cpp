/**
 * @copyright Copyright (c) 2019
 */


#include "simplepylearner.h"


SimplePyLearner::SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScript, double testProp):PyLearner(controller, learningScript, testProp){
    verifier = converter;
}

SimplePyLearner::~SimplePyLearner(){
    
}


template<class R, class T> double SimplePyLearner::intermediaryComputeError(const std::vector<R> input, const std::vector<T> output) const{
    auto v = verifier->computeServoToCoord(std::vector<uint16_t>(output.begin(), output.end()))->getCoord();
    std::cout << "Coordinates : "; for(auto val : v) std::cout << val << " "; std::cout << " instead of "; for(auto val : input) std::cout << val << " "; std::cout << std::endl;
    return TARGET_COEFF * computeSquaredError(input, verifier->computeServoToCoord(std::vector<uint16_t>(output.begin(), output.end()))->getCoord()) + MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
}
template double SimplePyLearner::intermediaryComputeError<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::intermediaryComputeError<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::intermediaryComputeError<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::intermediaryComputeError<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;


std::vector<double> SimplePyLearner::computeExpectedOutput(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const{ //TODO: make a better implementation for determining the corrected output
    double err = intermediaryComputeError(input, output);
    std::cout << "Error : " << err << std::endl;

    std::vector<double> res;
    std::vector<double> resMinus;
    for(auto ptr=output.cbegin(); ptr < output.cend(); ptr++){
        res.push_back(*ptr + err);
        resMinus.push_back(*ptr - err);
    }
    res = getClosestValidPosition(res);
    resMinus = getClosestValidPosition(resMinus);

    double resErr = intermediaryComputeError(input, res);
    double resMinusErr = intermediaryComputeError(input, resMinus);

    return (resErr < resMinusErr) ? res : resMinus;
}



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
    std::vector<double> expOutput;

    for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){
        std::cout << "Iteration " << nbIt << " : " << std::endl;
        output = pyLearn(fullInput, expOutput); // Computation of output
        expOutput = computeExpectedOutput(input.getInput(), output); // Computation of expected output
        std::cout << "Expected Output : "; for(auto v : expOutput) std::cout << v << " "; std::cout << std::endl;

        if(computeSquaredError(expOutput, output) < LEARN_ERROR_MARGIN) break; // Stop learning if corrections to do are negligeable
    }

    return new Output(output);
}


std::string SimplePyLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << PyLearner::toString();

    return rep.str();
}
/**
 * @copyright Copyright (c) 2019
 */


#include "bufferbasedpylearner.h"


BufferBasedPyLearner::BufferBasedPyLearner(AbstractController* controller, Converter* converter, std::string learningScriptSettings, double testProp):SimplePyLearner(controller, converter, learningScriptSettings, testProp){
    
}

BufferBasedPyLearner::~BufferBasedPyLearner(){
    
}



void BufferBasedPyLearner::learn(){

    auto lsetPtr = learningSet->begin(); // Learn trajectory for each example from the learning set

    for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){

        lsetPtr++; // Switch learning example at each iteration
        if(lsetPtr == learningSet->end()) lsetPtr = learningSet->begin();

        std::cout << "Reset device position..." << std::endl;
        device->goToBackhoe(); // Reset position
        device->waitFeedback();

        bool stop = false;
        int nbNullMove = 0;
        std::vector<State*> saves;

        // Attempts to reach target
        for(int nbMove = 0; nbMove < LEARN_NB_MOVEMENTS; nbMove++){
            std::cout << "Iteration " << nbIt << " - Move " << nbMove << " : " << std::endl;


            std::vector<uint16_t> fullInput(lsetPtr->first->getInput()); // Create input of the DNN, add the target coordinates
            auto state = DeviceLearner::getDeviceState(); // Get state of servomotors

            for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
                fullInput.insert(fullInput.end(), ptr->begin(), ptr->end()); // Add the current state of the servomotors to the input
            }
            
            std::cout << "Computing output..." << std::endl;
            auto output = pyCompute(fullInput); // Computation of output
            auto scaledOutput = formatOutput(output); // Format output for shipment to device

            std::cout << "Scaled output : [";
            for(auto ptr = scaledOutput.cbegin(); ptr < scaledOutput.cend(); ptr++) std::cout << *ptr << ", ";
            std::cout << "]" << std::endl;


            auto reward = computeReward(lsetPtr->first->getInput(), scaledOutput); // Computation of reward
            std::cout << "Reward : " << reward << std::endl;

            std::vector<double> rewardVector = {reward};
            saves.push_back(new State(fullInput, output, rewardVector));  // Save state
            
            
            if(reward > VALID_COEFF){ // If position is valid (within range)
                try{
                    device->setPosition(scaledOutput); // Update position
                    device->waitFeedback();

                    std::cout << "Updating position..." << std::endl;
                }catch(OutOfRangeError e){
                    std::cout << e.what() << std::endl;
                    nbNullMove++;
                }
            }else{
                std::cout << "Error too important : movement not allowed" << std::endl;
                nbNullMove++;
            }

            auto newPosition = device->getPosition(); // Display new position
            std::cout << "Current position : ";
            for(auto ptr = newPosition.cbegin(); ptr < newPosition.cend(); ptr++) {
                std::cout << *ptr << " ";
            }
            std::cout << std::endl;

            auto newCoords = verifier->computeServoToCoord(newPosition)->getCoord(); // Display corresponding new coordinates
            std::cout << "Current coordinates : ";
            for(auto ptr = newCoords.cbegin(); ptr < newCoords.cend(); ptr++) {
                std::cout << *ptr << " ";
            }
            std::cout << std::endl;
            
            /*
            if(abs(reward) < LEARN_ERROR_MARGIN) {  // Stop moving if error is within threshold
                std::cout << "reward value : " << reward << " smaller than " << LEARN_ERROR_MARGIN << ". End of learning..." << std::endl;
                stop = true;
                break;
            }
            //*/

            if(nbNullMove > MAX_NULL_MOVE) break; // Stop iteration if too many null movements
        }

        if(stop) break;
        
        // Update learner from its experience
        std::cout << "Backtracking reward to learner..." << std::endl;
        int i = 0;
        for(auto ptr = saves.rbegin(); ptr < saves.rend(); ptr++){
            
            std::vector<uint16_t> nextInput = (*(ptr+((ptr+1) != saves.rend())))->getInput(); // Send input of the next state reached with the given output (if no next input, is current input)
            pyLearn((*ptr)->getInput(), (*ptr)->getOutput(), (*ptr)->getReward(), nextInput, std::pow(DECREASING_REWARD, i)); // Decrease value of reward as the state is closer to initial state

            
            delete *ptr;
            i++;
        }
        std::cout << "Executed " << i << " updates..." << std::endl;
    }

    for(auto ptr = learningSet->begin(); ptr != learningSet->end(); ptr++){ // Produce output for each example from the learning set

        std::cout << "Reset device position..." << std::endl;
        device->goToBackhoe(); // Reset position
        device->waitFeedback();
        
        delete ptr->second;
        ptr->second = produce(*(ptr->first));

    }
}

void BufferBasedPyLearner::test(){

}



std::string BufferBasedPyLearner::toString() const{
    std::stringstream rep;
    rep << "Buffer-Based " << SimplePyLearner::toString();

    return rep.str();
}



BufferBasedPyLearner::State::State(std::vector<uint16_t>& inputVector, std::vector<double>& outputVector, std::vector<double>& reward){
    this->input = new std::vector<uint16_t>(inputVector);
    this->output = new std::vector<double>(outputVector);
    this->reward = new std::vector<double>(reward);
}

BufferBasedPyLearner::State::~State(){
    delete reward;
    delete output;
    delete input;
}


std::vector<uint16_t> BufferBasedPyLearner::State::getInput() const{
    return std::vector<uint16_t>(*input);
}

std::vector<double> BufferBasedPyLearner::State::getOutput() const{
    return std::vector<double>(*output);
}

std::vector<double> BufferBasedPyLearner::State::getReward() const{
    return std::vector<double>(*reward);
}

bool BufferBasedPyLearner::State::hasSameInput(State& s) const{
    return *(this->input) == *(s.input);
}
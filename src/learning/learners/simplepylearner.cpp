/**
 * @copyright Copyright (c) 2019
 */


#include "simplepylearner.h"


SimplePyLearner::SimplePyLearner(AbstractController* controller, Converter* converter, std::string learningScript, double testProp):PyLearner(controller, learningScript, testProp){
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

    //if(!device->validPosition(positionOutput)) return VALID_COEFF;
    
    auto oldCoords = verifier->computeServoToCoord(position)->getCoord();
    auto newCoords = verifier->computeServoToCoord(positionOutput)->getCoord();


    return TARGET_COEFF * (computeSquaredError(input, oldCoords) - computeSquaredError(input, newCoords)) - MOVEMENT_COEFF * computeSquaredError(device->getPosition(), output);
}
template double SimplePyLearner::computeReward<double, double>(const std::vector<double> input, const std::vector<double> output) const;
template double SimplePyLearner::computeReward<double, uint16_t>(const std::vector<double> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<uint16_t, double>(const std::vector<uint16_t> input, const std::vector<double> output) const;
template double SimplePyLearner::computeReward<uint16_t, uint16_t>(const std::vector<uint16_t> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<uint16_t, int>(const std::vector<uint16_t> input, const std::vector<int> output) const;
template double SimplePyLearner::computeReward<int, uint16_t>(const std::vector<int> input, const std::vector<uint16_t> output) const;
template double SimplePyLearner::computeReward<int, int>(const std::vector<int> input, const std::vector<int> output) const;


void SimplePyLearner::learn(){

    for(auto lsetPtr = learningSet->cbegin(); lsetPtr != learningSet->cend(); lsetPtr++){ // Learn trajectory for each example from the learning set

        for(int nbIt = 0; nbIt < LEARN_NB_ITERATIONS; nbIt++){
            std::cout << "Reset device position...";
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

                auto reward = computeReward(lsetPtr->first->getInput(), output); // Computation of reward
                std::cout << "Reward : " << reward << std::endl;
                std::vector<double> rewardVector = {reward};

                State* newState = new State(fullInput, output, rewardVector);  // Save state
                auto ptr = std::find_if(saves.begin(), saves.end(), [newState](State* s){ return (*s).hasSameInput(*newState); });
                if(ptr != saves.end()){
                    delete *ptr;
                    *ptr = newState;

                    nbNullMove++;
                }else{
                    saves.push_back(newState);
                }
                
                /*
                if(reward > VALID_COEFF){ // If position is valid (within range)
                    device->addPosition(output); // Update position
                    device->waitFeedback();

                    std::cout << "Updating position..." << std::endl;

                    auto pos = device->getPosition();
                    std::cout << "Resulting position : "; for(auto v : pos) std::cout << v << " "; std::cout << std::endl;
                    auto actualCoord = verifier->computeServoToCoord(pos)->getCoord();
                    auto targetCoord = lsetPtr->first->getInput();
                    std::cout << "Actual coordinates : "; for(auto v : actualCoord) std::cout << v << " "; std::cout << " instead of "; for(auto v : targetCoord)  std::cout << v << " "; std::cout << std::endl;
                }else{
                    std::cout << "Error too important : movement not allowed" << std::endl;
                }
                //*/

                //*
                try{
                    device->addPosition(output); // Update position
                    device->waitFeedback();

                    std::cout << "Updating position..." << std::endl;
                }catch(OutOfRangeError e){
                    std::cout << e.what() << std::endl;
                }
                //*/

                if(abs(reward) < LEARN_ERROR_MARGIN) {  // Stop moving if error is within threshold
                    std::cout << "reward value : " << reward << "smaller than " << LEARN_ERROR_MARGIN << ". End of learning..." << std::endl;
                    stop = true;
                    break;
                }

                if(nbNullMove > MAX_NULL_MOVE) break; // Stop iteration if too many null movements
            }

            if(stop) break;
            
            // Update learner from its experience
            std::cout << "Backtracking reward to learner..." << std::endl;
            int i = 0;
            for(auto ptr = saves.rbegin(); ptr < saves.rend(); ptr++){
                auto reward = (*ptr)->getReward();
                for(auto rewPtr = reward.begin(); rewPtr < reward.end(); rewPtr++) *rewPtr *= std::pow(DECREASING_REWARD, i); // Decrease value of reward as the state is closer to initial state
                
                pyLearn((*ptr)->getInput(), reward);

                delete *ptr;
                i++;
            }
            std::cout << "Executed " << i << " updates..." << std::endl;
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



SimplePyLearner::State::State(std::vector<uint16_t>& inputVector, std::vector<int>& outputVector, std::vector<double>& reward){
    this->input = new std::vector<uint16_t>(inputVector);
    this->output = new std::vector<int>(outputVector);
    this->reward = new std::vector<double>(reward);
}

SimplePyLearner::State::~State(){
    delete reward;
    delete output;
    delete input;
}


std::vector<uint16_t> SimplePyLearner::State::getInput() const{
    return std::vector<uint16_t>(*input);
}

std::vector<int> SimplePyLearner::State::getOutput() const{
    return std::vector<int>(*output);
}

std::vector<double> SimplePyLearner::State::getReward() const{
    return std::vector<double>(*reward);
}

bool SimplePyLearner::State::hasSameInput(State& s) const{
    return *(this->input) == *(s.input);
}
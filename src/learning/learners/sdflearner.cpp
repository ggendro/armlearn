/**
 * @copyright Copyright (c) 2019
 */


#include "sdflearner.h"





SdfLearner::SdfLearner(AbstractController* controller, Converter* converter):DeviceLearner(controller), rewards(LEARN_NB_MOVEMENTS), nbMoves(0){
    verifier = converter;
}

SdfLearner::~SdfLearner(){
    
}


double SdfLearner::computeReward(const std::vector<float> target, const std::vector<int> action) const{
    std::vector<uint16_t> positionOutput;
    auto position = device->getPosition();

    auto outPtr = action.cbegin();
    for(auto posPtr = position.cbegin(); posPtr < position.cend(); posPtr++) {
        positionOutput.push_back((uint16_t)(*posPtr + *outPtr));
        outPtr++;
    }
    
    if(!device->validPosition(positionOutput)) return VALID_COEFF;
    
    auto newCoords = verifier->computeServoToCoord(positionOutput)->getCoord();

    auto err = computeSquaredError(target, newCoords);
    if(abs(err) < LEARN_ERROR_MARGIN) return -VALID_COEFF;
    
    return -TARGET_COEFF * err - MOVEMENT_COEFF * computeSquaredError(device->getPosition(), action);
}


Converter* SdfLearner::getVerifier() const{
    return verifier;
}

AbstractController* SdfLearner::getDevice() const{
    return device;
}


void SdfLearner::init(float *state_observation, int state_space_size, float x_target, float y_target, float z_target){
    state_observation[0] = x_target;
    state_observation[1] = y_target;
    state_observation[2] = y_target;

    std::cout << "Reset device position..." << std::endl;
    device->goToBackhoe(); // Reset position
    device->waitFeedback();

    auto state = DeviceLearner::getDeviceState(); // Get state of servomotors
    
    int i=0;
    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        for(auto ptr2 = ptr->cbegin(); ptr2 < ptr->cend(); ptr2++){
            state_observation[i+3] = *ptr2;
            i++;
        }
    }
}

void SdfLearner::step(int state_space_size, int action_space_size, float x_target, float y_target, float z_target,
                            float *input_actions, float *state_observation, float *reward){
    
    if(nbMoves % LEARN_NB_MOVEMENTS == 0){
        float mean = 0;
        for(auto r : rewards) mean += r;
        mean /= LEARN_NB_MOVEMENTS;
        std::cout << "Mean reward : " << mean << std::endl;

        init(state_observation, state_space_size, x_target, y_target, z_target);
        std::cout << std::endl;
    }

    std::cout << "Iteration " << nbMoves / LEARN_NB_MOVEMENTS << " - Move " << nbMoves % LEARN_NB_MOVEMENTS << std::endl;
    
    // Computation of position
    std::vector<int> newPos;
    for(int i=0; i < action_space_size; i++) newPos.push_back(input_actions[i]);

    
    // Computation of reward
    float stepReward = computeReward({x_target, y_target, z_target}, newPos);
    rewards[nbMoves % LEARN_NB_MOVEMENTS] = stepReward;
    reward[0] = stepReward;

    if(stepReward > VALID_COEFF){ // If position is valid (within range)
        device->addPosition(newPos); // Update position
        device->waitFeedback();

        std::cout << "Updating position..." << std::endl;
    }else{
        std::cout << "Error too important : movement not allowed" << std::endl;
    }

    std::cout << "Action : "; for(auto v : newPos) std::cout << v << " "; std::cout << std::endl;
    std::cout << "Reward : " << stepReward << std::endl;
    auto position = device->getPosition();
    std::cout << "Position : "; for(auto v : position) std::cout << v << " "; std::cout << std::endl;
    std::cout << "Coordinates : "; for(auto v : verifier->computeServoToCoord(position)->getCoord()) std::cout << v << " "; std::cout << std::endl;

    // Update position for learner
    state_observation[0] = x_target;
    state_observation[1] = y_target;
    state_observation[2] = z_target;

    auto state = getDeviceState();
    int i=0;
    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) {
        for(auto ptr2 = ptr->cbegin(); ptr2 < ptr->cend(); ptr2++){
            state_observation[i+2] = *ptr2;
            i++;
        }
    }


    nbMoves++;
    std::cout << std::endl;
}



void SdfLearner::learn(){
    // TODO: to do
}

void SdfLearner::test(){
    // TODO: to do
}

Output<std::vector<uint16_t>>* SdfLearner::produce(const Input<uint16_t>& input){
    // TODO: to do
}


std::string SdfLearner::toString() const{
    std::stringstream rep;
    rep << "SDF " << DeviceLearner::toString();

    return rep.str();
}



extern "C" SdfLearner* initWrapper(float *state_observation, int state_space_size, float x_target, float y_target, float z_target) {
	Converter* conv = new OptimCartesianConverter();
	AbstractController* arbotix = new NoWaitArmSimulator((DisplayMode) 0);
	//AbstractController* arbotix = new SerialController("/dev/ttyUSB0");

	WidowXBuilder builder;
	builder.buildConverter(*conv);
	builder.buildController(*arbotix);

	arbotix->connect();
    arbotix->changeSpeed(50);
	arbotix->updateInfos();

    SdfLearner* learner = new SdfLearner(arbotix, conv);
    learner->init(state_observation, state_space_size, x_target, y_target, z_target);

    return learner;
}

extern "C" void stepWrapper(SdfLearner* env, 
                            int state_space_size, int action_space_size, float x_target, float y_target,float z_target,
                            float *input_actions, float *state_observation, float *reward) {

    env->step(state_space_size, action_space_size, x_target, y_target, z_target, input_actions, state_observation, reward);
}

extern "C" void endWrapper(SdfLearner* env) { // TODO: add call to this function in step when necessary
    Converter* conv = env->getVerifier();
    AbstractController* arbotix = env->getDevice();

    delete env;
    delete arbotix;
    delete conv;
}
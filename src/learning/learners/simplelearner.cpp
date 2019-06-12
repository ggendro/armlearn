/**
 * @copyright Copyright (c) 2019
 */


#include "simplelearner.h"


SimpleLearner::SimpleLearner(AbstractController* controller, Converter* converter, double testProp):DeviceLearner(controller, testProp){
    verifier = converter;
}

SimpleLearner::~SimpleLearner(){
    
}


void SimpleLearner::learn(){

}

void SimpleLearner::test(){

}

Output* SimpleLearner::produce(const Input& input){
    std::vector<uint16_t> fullInput(input.getInput());

    auto state = DeviceLearner::getDeviceState();
    for(auto ptr = state.cbegin(); ptr < state.cend(); ptr++) fullInput.insert(fullInput.end(), ptr->begin(), ptr->end());



    return new Output(fullInput);
}


std::string SimpleLearner::toString() const{
    std::stringstream rep;
    rep << "Simple " << DeviceLearner::toString();

    return rep.str();
}
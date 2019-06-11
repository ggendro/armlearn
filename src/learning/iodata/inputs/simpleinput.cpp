/**
 * @copyright Copyright (c) 2019
 */


#include "simpleinput.h"

SimpleInput::SimpleInput(const std::vector<uint16_t>& in):Input(in){

}

SimpleInput::SimpleInput(const SimpleInput& in):Input(in){
    
}

SimpleInput::~SimpleInput(){
    
}


std::string SimpleInput::toString() const{
    std::stringstream res;
    res << "Simple input : " << Input::toString();

    return res.str();
}
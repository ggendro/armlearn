/**
 * @copyright Copyright (c) 2019
 */


#include "simpleoutput.h"

SimpleOutput::SimpleOutput(const std::vector<uint16_t>& out):Output(out){
    
}

SimpleOutput::SimpleOutput(const SimpleOutput& out):Output(out){
    
}

SimpleOutput::~SimpleOutput(){
    
}


std::string SimpleOutput::toString() const{
    std::stringstream res;
    res << "Simple Output : " << Output::toString();

    return res.str();
}
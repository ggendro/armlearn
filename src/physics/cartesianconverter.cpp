/**
 * @copyright Copyright (c) 2019
 */


#include "cartesianconverter.h"


CartesianConverter::CartesianConverter():Converter(){

}

CartesianConverter::~CartesianConverter(){

}


Converter* CartesianConverter::computeServoToCoord(std::vector<uint16_t>& positions){
    lastInput = std::vector<uint16_t>(positions);

    // TODO: do computation


    return this;
}

Converter* CartesianConverter::computeCoordToServo(std::vector<uint16_t>& coordinates){
    lastOutput = std::vector<uint16_t>(coordinates);

    // TODO: do computation
    

    return this;
}



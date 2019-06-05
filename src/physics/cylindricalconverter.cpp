/**
 * @copyright Copyright (c) 2019
 */


#include "cylindricalconverter.h"


CylindricalConverter::CylindricalConverter():Converter(){

}

CylindricalConverter::~CylindricalConverter(){

}


Converter* CylindricalConverter::computeServoToCoord(std::vector<uint16_t>& positions){
    lastInput = std::vector<uint16_t>(positions);

    


    return this;
}

Converter* CylindricalConverter::computeCoordToServo(std::vector<uint16_t>& coordinates){
    lastOutput = std::vector<uint16_t>(coordinates);

    // TODO: do computation
    

    return this;
}



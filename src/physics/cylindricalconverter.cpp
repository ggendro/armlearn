/**
 * @copyright Copyright (c) 2019
 */


#include "cylindricalconverter.h"


CylindricalConverter::CylindricalConverter():Converter(){

}

CylindricalConverter::~CylindricalConverter(){

}


Converter* CylindricalConverter::computeServoToCoord(const std::vector<uint16_t>& positions){
    lastServo = std::vector<uint16_t>(positions);

    // TODO: do computation


    return this;
}

Converter* CylindricalConverter::computeCoordToServo(const std::vector<double>& coordinates){
    lastCoord = std::vector<double>(coordinates);

    // TODO: do computation
    

    return this;
}



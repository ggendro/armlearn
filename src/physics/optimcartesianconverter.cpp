/**
 * @copyright Copyright (c) 2019
 */


#include "optimcartesianconverter.h"

using namespace armlearn;
using namespace kinematics;


OptimCartesianConverter::OptimCartesianConverter():CartesianConverter(), rotatingBase(false), baseDefined(false){
    converters[0] = new BasicCartesianConverter();
    converters[1] = new CylindricalConverter();
}

OptimCartesianConverter::~OptimCartesianConverter(){
    delete converters[1];
    delete converters[0];
}


Converter* OptimCartesianConverter::addServo(const std::string& name, Axis axis, double lengthX, double lengthY, double lengthZ, double rotationX, double rotationY, double rotationZ){
    if(!baseDefined){
        rotatingBase = (axis == rotX || axis == rotY || axis == rotZ);
        baseDefined = true;
    }

    converters[rotatingBase]->addServo(name, axis, lengthX, lengthY, lengthZ, rotationX, rotationY, rotationZ);
    return this;
}

Converter* OptimCartesianConverter::removeAllServos(){
    converters[rotatingBase]->removeAllServos();
    baseDefined = false;

    return this;
}


std::vector<double> OptimCartesianConverter::convertCoordToCylindricalSystem(std::vector<double> cartCoord){
    if(cartCoord.size() != 3){
        std::stringstream errMsg;
        errMsg << "Input size " << cartCoord.size() << " is invalid : < 1";

        throw ComputationError(errMsg.str());
    }

    std::vector<double> res(3);

    res[0] = std::sqrt(std::pow(cartCoord[0], 2) + std::pow(cartCoord[1], 2));
    res[1] = std::atan(cartCoord[1] / cartCoord[0]);
    res[2] = cartCoord[2];


    return res;
}

std::vector<double> OptimCartesianConverter::convertCoordFromCylindricalSystem(std::vector<double> cylCoord){
    if(cylCoord.size() != 3){
        std::stringstream errMsg;
        errMsg << "Input size " << cylCoord.size() << " is invalid : < 1";

        throw ComputationError(errMsg.str());
    }

    std::vector<double> res(3);

    res[0] = cylCoord[0] * std::cos(cylCoord[1]);
    res[1] = cylCoord[0] * std::sin(cylCoord[1]);
    res[2] = cylCoord[2];


    return res;
}




Converter* OptimCartesianConverter::computeServoToCoord(const std::vector<uint16_t>& positions){

    // Save positions
    lastServo = std::vector<uint16_t>(positions);

    // Save coordinates
    lastCoord = converters[rotatingBase]->computeServoToCoord(positions)->getCoord();
    if(rotatingBase) lastCoord = convertCoordFromCylindricalSystem(lastCoord); // Need to convert from cylindrical to cartesian

    return this;
}

Converter* OptimCartesianConverter::computeCoordToServo(const std::vector<double>& coordinates){

    // Save coordinates
    lastCoord = std::vector<double>(coordinates);

    // Save positions
    lastServo = converters[rotatingBase]->computeCoordToServo(rotatingBase ? convertCoordToCylindricalSystem(coordinates) : coordinates)->getServo(); // Need to convert from cartesian to cylindrical

    return this;
}

int OptimCartesianConverter::getNbServos() const{
    return converters[rotatingBase]->getNbServos();
}



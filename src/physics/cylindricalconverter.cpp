/**
 * @copyright Copyright (c) 2019
 */


#include "cylindricalconverter.h"


CylindricalConverter::CylindricalConverter():Converter(), baseDefined(false){
    movingPart = new CartesianConverter();

}

CylindricalConverter::~CylindricalConverter(){
    delete movingPart;
}


Converter* CylindricalConverter::addServo(const std::string& name, Axis axis, double lengthX, double lengthY, double lengthZ, double rotationX, double rotationY, double rotationZ){
    if(baseDefined)
        movingPart->addServo(name, axis, lengthX, lengthY, lengthZ, rotationX, rotationY, rotationZ); 
        
    else{
        if(axis != rotX && axis != rotY && axis != rotZ){
            std::stringstream errMsg;
            errMsg << "Axis of the first servomotor has to be " << rotX << ", " << rotY << " or " << rotZ << " not " << axis;

            throw ConverterError(errMsg.str());
        }

        movingPart->addServo(name, fixed, lengthX, lengthY, lengthZ, rotationX, rotationY, rotationZ);
        baseAxis = axis;
        baseDefined = true;
    }
    
    return this;
}

Converter* CylindricalConverter::removeAllServos(){
    baseDefined = false;
    movingPart->removeAllServos();

    return this;
}




Converter* CylindricalConverter::computeServoToCoord(const std::vector<uint16_t>& positions){
    if(!baseDefined) throw ConverterError("Coordinate system not defined : add a rotational base");

    if(positions.size() < 1){
        std::stringstream errMsg;
        errMsg << "Input size " << positions.size() << " is invalid : < 1";

        throw ComputationError(errMsg.str());
    }

    // Angle formed by the basis
    double dTeta = TO_RADIAN(positions[0]);

    // If system is only composed of a basis
    if(positions.size() < 2){
        lastServo = std::vector<uint16_t>(positions);
        lastCoord = {dTeta};

        return this;
    }

    // Cartesian computation for the rest of the device
    std::vector<double> cartCoord = movingPart->computeServoToCoord(std::vector<uint16_t>(positions.begin()+1, positions.end()))->getCoord();


    // Save positions
    lastServo = std::vector<uint16_t>(positions);

    // Save coordinates
    lastCoord = {std::sqrt(std::pow(cartCoord[0], 2) + std::pow(cartCoord[1], 2)), (std::atan(cartCoord[1] / cartCoord[0]) + dTeta), cartCoord[2]};

    return this;
}

Converter* CylindricalConverter::computeCoordToServo(const std::vector<double>& coordinates){
    if(!baseDefined) throw ConverterError("Coordinate system not defined : add a rotational base");

    int spaceDim = 3;
    if(coordinates.size() != 3){
        std::stringstream errMsg;
        errMsg << "Input size " << coordinates.size() << " does not match the number of dimensions in the coordinates system : " << spaceDim;

        throw ComputationError(errMsg.str());
    }

    bool found = false;
    double granularity = M_PI / 6; // Granularity of the range checked
    double dTeta = coordinates[1];
    std::vector<uint16_t> movPos;
    std::string errMsg;

    for(double i = 0; i < 2 * M_PI && !found; i += granularity){ // Need to compute for a range of values of the basis
        try{
            movPos = movingPart->computeCoordToServo({coordinates[0] * std::cos(i), coordinates[0] * std::sin(i), coordinates[2]})->getServo();

            dTeta = dTeta - i;
            std::cout << "Teta : " << dTeta << std::endl;
            found = true;
        }catch(ComputationError e){ // Computation did not succeed, have to try again for another value of teta
            errMsg = e.what();
            
        }
    }


    if(!found) throw ComputationError(errMsg);
    

    // Save coordinates
    lastCoord = std::vector<double>(coordinates);

    // Save positions
    lastServo = std::vector<uint16_t>(movPos);
    lastServo.insert(lastServo.begin(), FROM_RADIAN(dTeta));

    return this;
}



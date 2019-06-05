/**
 * @copyright Copyright (c) 2019
 */


#include "cartesianconverter.h"


CartesianConverter::CartesianConverter():Converter(){
    cartesianConverter = new KDL::ChainFkSolverPos_recursive(*device);
    positionConverter = new KDL::ChainIkSolverPos_LMA(*device);
}

CartesianConverter::~CartesianConverter(){
    delete positionConverter;
    delete cartesianConverter;
}


Converter* CartesianConverter::computeServoToCoord(const std::vector<uint16_t>& positions){
    // Save  positions
    lastServo = std::vector<uint16_t>(positions);

    // Create input joint array
    int nbJoints = device->getNrOfJoints();
    if(nbJoints != positions.size()){
        std::stringstream errMsg;
        errMsg << "Input size " << positions.size() << " does not match the number of servomotors : " << nbJoints;

        throw ComputationError(errMsg.str());
    }
    KDL::JntArray jointPositions = KDL::JntArray(nbJoints);
    
    int i=0;
    for(auto ptr = positions.cbegin(); ptr < positions.cend(); ptr++){
        jointPositions(i)= TO_RADIAN((double) *ptr); // Conversion from servomotor unit to radian
        i++;
    }
 
    // Create the frame that will contain the results
    KDL::Frame cartPos;
 
    // Calculate forward position kinematics
    cartesianConverter->updateInternalDataStructures();
    int correct;
    correct = cartesianConverter->JntToCart(jointPositions, cartPos);
    if(correct < 0){
        throw ComputationError("Error : could not calculate forward kinematics");
    }

    // Save coordinates
    KDL::Vector res = cartPos.p;
    lastCoord = {res.x(), res.y(), res.z()};


    return this;
}

Converter* CartesianConverter::computeCoordToServo(const std::vector<double>& coordinates){
    // Save coordinates
    lastCoord = std::vector<double>(coordinates);

    // Create input frame
    int spaceDim = 3;
    if(coordinates.size() != 3){
        std::stringstream errMsg;
        errMsg << "Input size " << coordinates.size() << " does not match the number of dimensions in the coordinates system : " << spaceDim;

        throw ComputationError(errMsg.str());
    }
    KDL::Frame cartPos(KDL::Vector(coordinates[0], coordinates[1], coordinates[2]));

    // Create initial joint array from last known position of the device
    KDL::JntArray initialValues(device->getNrOfJoints());
    int i=0;
    for(auto ptr = lastServo.cbegin(); ptr < lastServo.cend(); ptr++){
        initialValues(i)= TO_RADIAN((double) *ptr); // Conversion from servomotor unit to radian
        i++;
    }

    // Create the joint array that will contain the results
    KDL::JntArray jointPositions(device->getNrOfJoints());

    // Calculate inverse position kinematics
    positionConverter->updateInternalDataStructures();
    int correct;
    correct = positionConverter->CartToJnt(initialValues, cartPos, jointPositions);
    if(correct < 0){
        std::stringstream errMsg;
        errMsg << "Error " << correct << " : could not calculate inverse kinematics";

        throw ComputationError(errMsg.str());
    }

    // Save positions
    lastServo = {};
    for(int i=0; i < jointPositions.rows(); i++){
        lastServo.push_back(TO_RADIAN(jointPositions(i))); // Conversion from radian to servomotor unit
    }


    return this;
}



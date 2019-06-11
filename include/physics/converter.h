/**
 * @file converter.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract Converter class
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef CONVERTER_H
#define CONVERTER_H

#include <kdl/chain.hpp>
#include <kdl/segment.hpp>
#include <vector>
#include <iostream>

#include "range.h"
#include "computationerror.h"


/**
 * @brief Enumeration listing the axis possibilities of each servomotor
 *  - rotation along X, Y or Z axis
 *  - translation along X, Y or Z axis
 *  - if fixed, does not count as a servomotor
 */
enum Axis{
    rotX,
    rotY,
    rotZ,
    transX,
    transY,
    transZ,
    fixed
};


/**
 * @class Converter
 * @brief Abstract class computing servomotor positions into a coordinate system and reciprocally
 * 
 */
class Converter{

    protected:
        std::vector<uint16_t> lastServo;
        std::vector<double> lastCoord;

        KDL::Chain* device;

    public:

        /**
         * @brief Constructs a new Converter object
         * 
         */
        Converter();

        /**
         * @brief Destroys the Converter object
         * 
         */
        virtual ~Converter();


        /**
         * @brief Adds an arm part to the robotic device, composed of a servomotor rotating along an axis linked to a rigid part of a given length
         * 
         * @param id the id of the servomotor
         * @param name the name of the servomotor
         * @param axis along which axis does the servomotor rotate 
         * @param lengthX length of the rigid part along the X axis (translating along X its inner frame), in arbitrary unit
         * @param lengthY length of the rigid part along the Y axis (translating along Y its inner frame), in arbitrary unit
         * @param lengthZ length of the rigid part along the Z axis (translating along Z its inner frame), in arbitrary unit
         * @param rot initial rotation of the servomotor along its rotation axis (rotating its inner frame), in radian
         * @return Converter* pointor to itself, to be able to chain computations (as in functional programming)
         */
        virtual Converter* addServo(const std::string& name, Axis axis = fixed, double lengthX = 0.0, double lengthY = 0.0, double lengthZ = 0.0, double rotationX = 0.0, double rotationY = 0.0, double rotationZ = 0.0);

        /**
         * @brief Resets device system
         * 
         * @return Converter* pointor to itself, to be able to chain computations (as in functional programming)
         */
        virtual Converter* removeAllServos();


        /**
         * @brief Computes coordinates from positions of widowx arm servomotors
         * 
         * @param positions the input of the calculation, positions of the servomotors
         * @return Converter* pointor to itself, to be able to chain computations (as in functional programming)
         * 
         * virtual method, inherited classes will implement calculations for specific coordinates systems
         */
        virtual Converter* computeServoToCoord(const std::vector<uint16_t>& positions) = 0;

        /**
         * @brief Computes positions of widowx arm servomotors from coordinates (according to the coordinate system implemented)
         * 
         * @param coordinates the input of the calculation, coordinates of the arm
         * @return Converter* pointor to itself, to be able to chain computations (as in functional programming)
         * 
         * virtual method, inherited classes will implement calculations for specific coordinates systems
         */
        virtual Converter* computeCoordToServo(const std::vector<double>& coordinates) = 0;


        /**
         * @brief Gets the coordinates of the last computation
         * 
         * @return std::vector<double> the coordinates
         */
        std::vector<double> getCoord() const;

        /**
         * @brief Gets the servo positions of the last computation
         * 
         * @return std::vector<uint16_t> the positions
         */
        std::vector<uint16_t> getServo() const;

};


#endif
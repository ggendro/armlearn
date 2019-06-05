/**
 * @file converter.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
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
#include <kdl/chainiksolver.hpp>
#include <vector>
#include <iostream>

#include "computationerror.h"


/**
 * @brief Class computing servomotor positions into a coordinate system and reciprocally
 * 
 */
class Converter{

    protected:
        std::vector<uint16_t> lastInput;
        std::vector<uint16_t> lastOutput;

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
         * @brief Computes coordinates from positions of widowx arm servomotors
         * 
         * @param positions the input of the calculation, positions of the servomotors
         * @return Converter* pointor to itself, to be able to chain computations (as in functional prgramming)
         * 
         * virtual method, inherited classes will implement calculations for specific coordinates systems
         */
        virtual Converter* computeServoToCoord(std::vector<uint16_t>& positions) = 0;

        /**
         * @brief Computes positions of widowx arm servomotors from coordinates (according to the coordinate system implemented)
         * 
         * @param coordinates the input of the calculation, coordinates of the arm
         * @return Converter* pointor to itself, to be able to chain computations (as in functional prgramming)
         * 
         * virtual method, inherited classes will implement calculations for specific coordinates systems
         */
        virtual Converter* computeCoordToServo(std::vector<uint16_t>& coordinates) = 0;


        /**
         * @brief Gets the output of the last computation
         * 
         * @return std::vector<uint16_t> the output
         */
        std::vector<uint16_t> get();

        /**
         * @brief Gets the input of the last computation
         * 
         * @return std::vector<uint16_t> the input
         */
        std::vector<uint16_t> getInput();

};


#endif
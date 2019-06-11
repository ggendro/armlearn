/**
 * @file basiccartesianconverter.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the BasicCartesianConverter class, inherited from CartesianConverter class
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef BASICCARTESIANCONVERTER_H
#define BASICCARTESIANCONVERTER_H

#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolverpos_nr.hpp>

#include "cartesianconverter.h"


/**
 * @class BasicCartesianConverter
 * @brief Class computing servomotor positions into cartesian coordinate system and reciprocally
 * 
 * Note that computations are more likely to succeed if the device only moves within a plane 
 *  
 */
class BasicCartesianConverter : public CartesianConverter{

    protected:
        KDL::ChainFkSolverPos* cartesianConverter;

        KDL::ChainIkSolverVel* intermediaryVelocitySolver;
        KDL::ChainIkSolverPos* positionConverter;


    public:

        /**
         * @brief Constructs a new Cartesian Converter object
         * 
         */
        BasicCartesianConverter();

        /**
         * @brief Destroys the Cartesian Converter object
         * 
         */
        virtual ~BasicCartesianConverter();


        /**
         * @brief Computes cartesian coordinates from servomotor positions
         * 
         * @param positions the positions of the servomotors
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeServoToCoord(const std::vector<uint16_t>& positions) override;

        /**
         * @brief Computes servomotor positions from cartesian coordinates
         * 
         * @param coordinates under cartesian coordinate system [X, Y, Z] 
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeCoordToServo(const std::vector<double>& coordinates) override;

};


#endif
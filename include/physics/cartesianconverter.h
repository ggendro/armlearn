/**
 * @file cartesianconverter.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the CartesianConverter class, inherited from Converter class
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef CARTESIANCONVERTER_H
#define CARTESIANCONVERTER_H

#include "converter.h"


/**
 * @brief Class computing servomotor positions into cartesian coordinate system and reciprocally
 * 
 */
class CartesianConverter : public Converter{

    public:

        /**
         * @brief Constructs a new Cartesian Converter object
         * 
         */
        CartesianConverter();

        /**
         * @brief Destroys the Cartesian Converter object
         * 
         */
        virtual ~CartesianConverter();


        /**
         * @brief Computes cartesian coordinates from servomotor positions
         * 
         * @param positions the positions of the servomotors
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeServoToCoord(std::vector<uint16_t>& positions) override;

        /**
         * @brief Computes servomotor positions from cartesian coordinates
         * 
         * @param coordinates under cartesian coordinate system [X, Y, Z] 
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeCoordToServo(std::vector<uint16_t>& coordinates) override;

};


#endif
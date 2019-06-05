/**
 * @file cylindricalconverter.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the CylindricalConverter class, inherited from Converter class
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef CYLINDRICALCONVERTER_H
#define CYLINDRICALCONVERTER_H

#include "converter.h"


/**
 * @brief Class computing servomotor positions into cylindrical coordinate system and reciprocally
 * 
 */
class CylindricalConverter : public Converter{

    public:

        /**
         * @brief Constructs a new Cylindrical Converter object
         * 
         */
        CylindricalConverter();

        /**
         * @brief Destroys the Cylindrical Converter object
         * 
         */
        virtual ~CylindricalConverter();


        /**
         * @brief Computes cylindrical coordinates from servomotor positions using Inverse Kinematics
         * 
         * @param positions the positions of the servomotors
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeServoToCoord(std::vector<uint16_t>& positions) override;

        /**
         * @brief Computes servomotor positions from cylindrical coordinates using Forward Kinematics
         * 
         * @param coordinates under cylindrical coordinate system [R, Têta, Z] 
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeCoordToServo(std::vector<uint16_t>& positions) override;

};


#endif
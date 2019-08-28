/**
 * @file cylindricalconverter.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the CylindricalConverter class, inherited from Converter class
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef CYLINDRICALCONVERTER_H
#define CYLINDRICALCONVERTER_H

#include <math.h>

#include "basiccartesianconverter.h"
#include "convertererror.h"

namespace armlearn {
    namespace kinematics {

/**
 * @class CylindricalConverter
 * @brief Class computing servomotor positions into cylindrical coordinate system and reciprocally
 * 
 * Note that the first servo must have a rotation axis in order to set the cylindrical coordinate system
 * 
 */
class CylindricalConverter : public Converter{

    protected:
        BasicCartesianConverter* movingPart;

        Axis baseAxis;
        bool baseDefined;


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
         * 
         * @throw ConverterError if the first servomotor added is not along a rotating axis, required to set the coordinate system
         * Redefinition of Converter method
         */
        virtual Converter* addServo(const std::string& name, Axis axis = fixed, double lengthX = 0.0, double lengthY = 0.0, double lengthZ = 0.0, double rotationX = 0.0, double rotationY = 0.0, double rotationZ = 0.0) override;

        /**
         * @brief Resets device system
         * 
         * @return Converter* pointor to itself, to be able to chain computations (as in functional programming)
         * 
         * Redefinition of Converter method
         */
        virtual Converter* removeAllServos() override;


        /**
         * @brief Computes cylindrical coordinates from servomotor positions using Inverse Kinematics
         * 
         * @param positions the positions of the servomotors
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeServoToCoord(const std::vector<uint16_t>& positions) override;

        /**
         * @brief Computes servomotor positions from cylindrical coordinates using Forward Kinematics
         * 
         * @param coordinates under cylindrical coordinate system [R, Têta, Z] 
         * @return Converter* itself (see converter.h)
         */
        virtual Converter* computeCoordToServo(const std::vector<double>& coordinates) override;

};

    }
}

#endif
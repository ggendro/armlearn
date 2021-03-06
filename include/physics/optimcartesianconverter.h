/**
 * @file optimcartesianconverter.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the OptimCartesianConverter class, inherited from CartesianConverter class
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OPTIMCARTESIANCONVERTER_H
#define OPTIMCARTESIANCONVERTER_H

#include "cartesianconverter.h"
#include "cylindricalconverter.h"

namespace armlearn {
    namespace kinematics {

/**
 * @class OptimCartesianConverter
 * @brief Class computing servomotor positions into cartesian coordinate system and reciprocally, can optimize computations if basis is cylindrical
 * 
 */
class OptimCartesianConverter : public CartesianConverter{

    protected:
        Converter* converters[2];

        bool rotatingBase;
        bool baseDefined;

        /**
         * @brief Converts coordinates in cartesian system into coordinates in cylindrical system
         * 
         * @param cartCoord input coordinates in cartesian system
         * @return std::vector<double> output coordinates in cylindrical system
         */
        std::vector<double> convertCoordToCylindricalSystem(std::vector<double> cartCoord);

        /**
         * @brief Converts coordinates in cylindrical system into coordinates in cartesian system
         * 
         * @param cylCoord input coordinates in cylindrical system
         * @return std::vector<double> output coordinates in cartesian system
         */
        std::vector<double> convertCoordFromCylindricalSystem(std::vector<double> cylCoord);


    public:

        /**
         * @brief Constructs a new Optimized Cartesian Converter object
         * 
         */
        OptimCartesianConverter();

        /**
         * @brief Destroys the Optimized Cartesian Converter object
         * 
         */
        virtual ~OptimCartesianConverter();


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
         * if the first servomotor added is along a rotating axis, will use cylindrical computations and simply convert the result into a cartesian coordinate system
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

        /**
         * @brief Gets the number of moveable servomotors used in the computation (i.e: size of the getServo() output)
         * 
         * @return int the number of servomotors
         */
        virtual int getNbServos() const override;

};

    }
}


#endif
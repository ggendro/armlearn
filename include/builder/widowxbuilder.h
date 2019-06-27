/**
 * @file widowxbuilder.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class WidowXBuilder, used to construct the main classes of the library for the widowx arm (see documentation at https://learn.trossenrobotics.com/38-interbotix-robots/124-widowx-robot-arm.html)
 * @version 0.1
 * @date 2019-06-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef WIDOWXBUILDER_H
#define WIDOWXBUILDER_H

#include "builder.h"

/**
 * @class WidowXBuilder
 * @brief Class for building the main classes of the library (Controller, Converter, ...) for he the WidowX arm device
 * 
 * Lengths based on the values given in the Arm Link protocol and defined here https://github.com/Interbotix/ArmLinkLibrary/blob/master/ArmLink/examples/InterbotixArmLinkSerial/GlobalArm.h
 */
class WidowXBuilder : public Builder{

    public:
        /**
         * @brief Constructs a new WidowXBuilder object
         * 
         */
        WidowXBuilder();

        /**
         * @brief Destroys the WidowXBuilder object
         * 
         */
        virtual ~WidowXBuilder();

        /**
         * @brief Adds to a controller all the elements relative to the WidowX arm device
         * 
         * @param controller the controller to build
         */
        virtual void buildController(AbstractController& controller) override;

        /**
         * @brief Adds to a converter all the elements relative to the WidowX arm device
         * 
         * @param converter the converter to build
         */
        virtual void buildConverter(Converter& converter) override;

};


#endif
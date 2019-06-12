/**
 * @file builder.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class Builder, used to construct the main classes of the library for specific devices
 * @version 0.1
 * @date 2019-06-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef BUILDER_H
#define BUILDER_H

#include "abstractcontroller.h"
#include "converter.h"
#include "learner.h"

/**
 * @class Builder
 * @brief Abstract class for building the main classes of the library (Controller, Converter, ...) for a specific device such as the WidowX arm
 * 
 */
class Builder{

    public:

        /**
         * @brief Adds to a controller all the elements relative to the specific device linked to the builder
         * 
         * @param controller the controller to build
         * 
         * Abstract method, implemented in inherited classes
         */
        virtual void buildController(AbstractController& controller) = 0;

        /**
         * @brief Adds to a converter all the elements relative to the specific device linked to the builder
         * 
         * @param converter the converter to build
         * 
         * Abstract method, implemented in inherited classes 
         */
        virtual void buildConverter(Converter& converter) = 0;

};


#endif
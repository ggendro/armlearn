/**
 * @file cartesianconverter.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract CartesianConverter class, inherited from Converter class
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef CARTESIANCONVERTER_H
#define CARTESIANCONVERTER_H

#include "converter.h"

namespace armlearn {
    namespace kinematics {

/**
 * @class CartesianConverter
 * @brief Abstract class for cartesian converters
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

};

    }
}


#endif
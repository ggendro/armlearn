/**
 * @file input.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class Input, input used by learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef INPUT_H
#define INPUT_H

#include "iovector.h"

namespace armlearn {

/**
 * @class Input
 * @brief Input of learning algorithms, inherited from IOVector
 */
template<class T> class Input : public IOVector<T>{

    public:
        /**
         * @brief Destroy the Input object
         * 
         */
        using IOVector<T>::IOVector;
        virtual ~Input(){}

        /**
         * @brief Returns the input under the form of a vector useable for most of classifiers or learning structures
         * 
         * @return std::vector<T> the input
         */
        virtual std::vector<T> getInput() const{
            return IOVector<T>::vector;
        }
};

}

#endif
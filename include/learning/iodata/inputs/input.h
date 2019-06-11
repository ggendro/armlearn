/**
 * @file input.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class Input, input used by learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef INPUT_H
#define INPUT_H

#include "iovector.h"

/**
 * @class Input
 * @brief Input of learning algorithms, inherited from IOVector
 * 
 * Does not correspond to a real output, used as mother class for other classes
 */
class Input : public IOVector{

    public:
        using IOVector::IOVector;
        virtual ~Input();

        /**
         * @brief Returns the input under the form of a vector useable for most of classifiers or learning structures
         * 
         * @return std::vector<uint16_t> the input
         */
        virtual std::vector<uint16_t> getInput();
};



#endif
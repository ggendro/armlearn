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

#include <vector>

/**
 * @class Input
 * @brief Input of learning algorithms
 */
class Input{

    public:

        /**
         * @brief Constructs a new Input object
         * 
         */
        Input();

        /**
         * @brief Destroys the Input object
         * 
         */
        virtual ~Input();


        /**
         * @brief Returns the Input under the form of a vector useable for most of classifiers or learning structures
         * 
         * @return std::vector<uint16_t> the input
         * 
         * Pseudo virtual method, implemented in inherited classes, this one returns an empty vector
         */
        virtual std::vector<uint16_t> getVector(){ return std::vector<uint16_t>(); }
};

#endif
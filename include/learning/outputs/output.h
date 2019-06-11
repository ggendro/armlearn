/**
 * @file output.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class Output, output used by learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>

/**
 * @class Output
 * @brief Output of learning algorithms
 */
class Output{ 

    public:

        /**
         * @brief Constructs a new Output object
         * 
         */
        Output();

        /**
         * @brief Destroys the Output object
         * 
         */
        virtual ~Output();


        /**
         * @brief Returns the output under the form of an array of intermediary positions
         * 
         * @return std::vector<std::vector<uint16_t>> the output
         * 
         * Pseudo virtual method, implemented in inherited classes, this one returns an empty vector
         */
        virtual std::vector<std::vector<uint16_t>> getPositions(){ return std::vector<std::vector<uint16_t>>(); }
};

#endif
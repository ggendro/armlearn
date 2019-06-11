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

#include "iovector.h"

/**
 * @class Output
 * @brief Output of learning algorithms, inherited from IOvector
 * 
 * Does not correspond to a real output, used as mother class for other classes
 */
class Output : public IOVector{

    public:
        using IOVector::IOVector;
        virtual ~Output();


        /**
         * @brief Returns the output under the form of an array of intermediary positions
         * 
         * @return std::vector<std::vector<uint16_t>> the output
         */
        virtual std::vector<std::vector<uint16_t>> getOutput();
};

#endif
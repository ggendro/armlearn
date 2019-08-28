/**
 * @file output.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class Output, output used by learners classes
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef OUTPUT_H
#define OUTPUT_H

#include "iovector.h"

namespace armlearn {

/**
 * @class Output
 * @brief Output of learning algorithms, inherited from IOvector
 */
template<class T> class Output : public IOVector<T>{

    public:
        /**
         * @brief Destroy the Output object
         * 
         */
        using IOVector<T>::IOVector;
        virtual ~Output(){}


        /**
         * @brief Returns the output under the form of an array of intermediary positions
         * 
         * @return std::vector<std::vector<T>> the output
         */
        virtual std::vector<T> getOutput() const{
            return IOVector<T>::vector;
        }
};

}

#endif
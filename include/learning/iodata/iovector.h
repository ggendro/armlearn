/**
 * @file iovector.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the abstract class IOvector, representing input or output vectors from learning
 * @version 0.1
 * @date 2019-06-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef IOVECTOR_H
#define IOVECTOR_H

#include <vector>
#include <iostream>
#include <sstream>

/**
 * @class IOVector
 * @brief Inout or Output of learning algorithms
 */
class IOVector{

    protected:
        std::vector<uint16_t> vector;

    public:

        /**
         * @brief Constructs a new IOVector object
         * 
         */
        IOVector();

        /**
         * @brief Constructs a new IOVector object
         * 
         */
        IOVector(const std::vector<uint16_t>& vect);

        /**
         * @brief Constructs a new IOVector object from another
         * 
         * @param vect the vector to copy
         */
        IOVector(const IOVector& vect);

        /**
         * @brief Destroys the IOVector object
         * 
         */
        virtual ~IOVector();


        /**
         * @brief assignement operator
         * 
         * @param out the in copied in the current object
         * @return IOVector& the current IOVector object
         */
        virtual IOVector& operator=(const IOVector& out);

        /**
         * @brief equals comparison operator
         * 
         * @param obj object to compare to
         * @return true if the vectors are identical
         * @return false otherwise
         */
        virtual bool operator==(const IOVector& obj);


        /**
         * @brief Returns the vector
         * 
         * @return std::vector<uint16_t> the contained vector
         */
        std::vector<uint16_t> getVector();


        /**
         * @brief Returns the vector under string format
         * 
         * @return std::string
         */
        virtual std::string toString() const;
};

#endif
/**
 * @file iovector.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the class IOvector, representing input or output vectors from learning
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
 * @brief Input or Output of learning algorithms
 */
template<class T> class IOVector{

    protected:
        std::vector<T> vector;

    public:

        /**
         * @brief Constructs a new IOVector object
         * 
         */
        IOVector(){ }

        /**
         * @brief Constructs a new IOVector object
         * 
         */
        IOVector(const std::vector<T>& vect){
            vector = vect;
        }

        /**
         * @brief Constructs a new IOVector object from another
         * 
         * @param vect the vector to copy
         */
        IOVector(const IOVector& vect){
            vector = vect.vector;
        }

        /**
         * @brief Destroys the IOVector object
         * 
         */
        virtual ~IOVector(){ }


        /**
         * @brief assignement operator
         * 
         * @param out the in copied in the current object
         * @return IOVector& the current IOVector object
         */
        virtual IOVector& operator=(const IOVector& obj){
            vector = obj.vector;

            return *this;
        }

        /**
         * @brief equals comparison operator
         * 
         * @param obj object to compare to
         * @return true if the vectors are identical
         * @return false otherwise
         */
        virtual bool operator==(const IOVector& obj) const{
            return vector == obj.vector;
        }


        /**
         * @brief Returns the vector
         * 
         * @return std::vector<uint16_t> the contained vector
         */
        std::vector<T> getVector() const{
            return vector;
        }


        /**
         * @brief Returns the vector under string format
         * 
         * @return std::string
         */
        virtual std::string toString() const{
            std::stringstream res;
            res << "{ ";
            for(auto&& v : vector) res << v << " ";
            res << "}";

            return res.str();
        }
};


/**
 * @brief Redefinition of operator<< for vector
 * 
 * @tparam U type of the vector content
 * @param os the stream to print the vector
 * @param vect the vector to print
 * @return std::ostream& the stream with the printed vector
 */
template <typename U> std::ostream& operator<<(std::ostream& os, const std::vector<U>& vect){ 
    os << "{";
    for(auto&& v : vect) os << v << " ";
    os << "}"; 
    return os; 
}

#endif
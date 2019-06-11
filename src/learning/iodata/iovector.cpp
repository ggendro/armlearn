/**
 * @copyright Copyright (c) 2019
 */


#include "iovector.h"

IOVector::IOVector(){
    
}

IOVector::IOVector(const std::vector<uint16_t>& vect){
    vector = vect;
}

IOVector::IOVector(const IOVector& vect){
    vector = vect.vector;
}

IOVector::~IOVector(){
    
}


IOVector& IOVector::operator=(const IOVector& obj){
    vector = obj.vector;

    return *this;
}

bool IOVector::operator==(const IOVector& obj){
    return vector == obj.vector;
}


std::vector<uint16_t> IOVector::getVector(){
    return vector;
}


std::string IOVector::toString() const{
    std::stringstream res;
    res << "{ ";
    for(auto&& v : vector) res << v << " ";
    res << "}";

    return res.str();
}
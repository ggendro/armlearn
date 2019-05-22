/**
 * @copyright Copyright (c) 2019
 * 
 */

#include "range.h"

/**
 * @brief Construct a new Range:: Range object
 * 
 * @param min the minimum value of the range
 * @param max the maximum value of the range
 * @param dom the domain on which this range applies, one required, several can be added
 * @param name the name of the corresponding servomotor
 */
Range::Range(int min, int max, Mode dom, std::string& name):minValue(min), maxValue(max), motorName(name){
    this->domains = new std::vector<Mode>();
    this->domains->push_back(dom);
}

/**
 * @brief Destroy the Range:: Range object
 * 
 */
Range::~Range(){
    delete domains;
}

/**
 * @brief add a domain to the range
 * 
 * @param domain the domain to add (cartesian, cylindric, backhoe, ...)
 * @return true if the domain has correctly been added
 * @return false if the domain was already added
 */
bool Range::addDomain(Mode domain){
    for(auto ptr=this->domains->cbegin(); ptr < this->domains->cend(); ptr++){
        if (*ptr == domain) return false;
    }

    this->domains->push_back(domain);
    return true;
}

/**
 * @brief remove a domain from the range
 * 
 * @param domain the domain to remove (cartesian, cylindric, backhoe, ...)
 * @return true if the domain has correctly been removed
 * @return false if the domain was not present in the range
 */
bool Range::removeDomain(Mode domain){
    for(auto ptr=this->domains->begin(); ptr < this->domains->end(); ptr++){
        if (*ptr == domain){
            this->domains->erase(ptr);
            return true;
        }
    }

    return false;
}

/**
 * @brief Check if the value in parameter is within the range
 * 
 * @param value the value to check
 * @return true if the value is valid
 * @return false otherwise
 */
bool Range::isValid(int value) const{
    return value < this->maxValue && value > this->minValue;
}

/**
 * @brief Check if the range applies in the domain passed in parameter
 * 
 * @param domain the domain to verify
 * @return true if the domain is in the range's list
 * @return false otherwise
 */
bool Range::hasDomain(Mode domain) const{

    for(auto ptr=this->domains->cbegin(); ptr < this->domains->cend(); ptr++){
        if (*ptr == domain){
            return true;
        }
    }

    return false;
}

std::string Range::getName() const{
    return this->motorName;
}
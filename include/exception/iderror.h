/**
 * @file iderror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exeption thrown when an incorrect id is used
 * @version 0.1
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef IDERROR_H
#define IDERROR_H

#include <stdexcept>

class IdError : public std::runtime_error{
    public:
        IdError();
        IdError(const std::string& what_arg);
        IdError(const char* what_arg);
};

#endif
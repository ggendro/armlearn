/**
 * @file outofrangeerror.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Exception thrown when trying to set a servomotor's value out of its range
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OUTOFRANGEERROR_H
#define OUTOFRANGEERROR_H

#include <stdexcept>

class OutOfRangeError : public std::range_error{
    public:
        OutOfRangeError();
        OutOfRangeError(const std::string& what_arg);
        OutOfRangeError(const char* what_arg);
};

#endif
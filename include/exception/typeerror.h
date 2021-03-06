/**
 * @file typeerror.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Exception thrown when initializing a servomotor's type
 * @version 0.1
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TYPEERROR_H
#define TYPEERROR_H

#include <stdexcept>

namespace armlearn {

class TypeError : public std::runtime_error{
    public:
        TypeError();
        TypeError(const std::string& what_arg);
        TypeError(const char* what_arg);
};

}

#endif
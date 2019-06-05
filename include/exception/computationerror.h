/**
 * @file computationerror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exception thrown when error occurs during a computation
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef COMPUTATIONERROR_H
#define COMPUTATIONERROR_H

#include <stdexcept>

class ComputationError : public std::runtime_error{
    public:
        ComputationError();
        ComputationError(const std::string& what_arg);
        ComputationError(const char* what_arg);
};

#endif
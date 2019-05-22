/**
 * @file movementerror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exception threw when trying to move the device when not available
 * @version 0.1
 * @date 2019-05-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MOVEMENTERROR_H
#define MOVEMENTERROR_H

#include <stdexcept>

class MovementError : public std::runtime_error{
    public:
        MovementError();
        MovementError(const std::string& what_arg);
        MovementError(const char* what_arg);
};

#endif
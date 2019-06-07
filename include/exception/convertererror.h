/**
 * @file convertererror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exception thrown when an error occured while trying to set a converter
 * @version 0.1
 * @date 2019-06-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef COVERTERERROR_H
#define COVERTERERROR_H

#include <stdexcept>

class ConverterError : public std::runtime_error{
    public:
        ConverterError();
        ConverterError(const std::string& what_arg);
        ConverterError(const char* what_arg);
};

#endif
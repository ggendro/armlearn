/**
 * @file connectionerror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exception threw when error raised during connection establishment
 * @version 0.1
 * @date 2019-05-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef CONNECTIONERROR_H
#define CONNECTIONERROR_H

#include <stdexcept>

class ConnectionError : public std::runtime_error{
    public:
        ConnectionError();
        ConnectionError(const std::string& what_arg);
        ConnectionError(const char* what_arg);
};

#endif
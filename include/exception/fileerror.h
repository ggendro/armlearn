/**
 * @file file.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Exception thrown when an error occured while trying to manage a file
 * @version 0.1
 * @date 2019-13-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef FILEERROR_H
#define FILEERROR_H

#include <stdexcept>

class FileError : public std::runtime_error{
    public:
        FileError();
        FileError(const std::string& what_arg);
        FileError(const char* what_arg);
};

#endif
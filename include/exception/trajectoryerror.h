/**
 * @file trajectoryerror.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Exception thrown when an error occured during the execution of a trajectory
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TRAJECTORYERROR_H
#define TRAJECTORYERROR_H

#include <stdexcept>

class TrajectoryError : public std::runtime_error{
    public:
        TrajectoryError();
        TrajectoryError(const std::string& what_arg);
        TrajectoryError(const char* what_arg);
};

#endif
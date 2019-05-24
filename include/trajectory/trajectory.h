/**
 * @file trajectory.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Provides an interface to send trajectories to WidowX robot in backhoe mode
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <stdexcept>

#include "widowx.h"
#include "range.h"
#include "movementerror.h"
#include "trajectoryerror.h"


#define WAITING_TIME 1000


/**
 * @brief class Trajectory
 * 
 */
class Trajectory{

    private:
        WidowX* device;
        std::vector<std::vector<int>*>* trajectories;
        // TODO: add time management (pauses during execution, varying speed of servomotors, ...)

        bool waitFeedback() const;

    public:
        Trajectory(WidowX* toDevice);
        virtual ~Trajectory(); // Does not delete the WidowX device to be able to reuse it to launch other trajectories

        void addPoint(const std::vector<int>& point, int pos);
        void addPoint(const std::vector<int>& point);
        void removePoint(int pos);
        void removePoint();

        void init() const;
        void executeTrajectory() const;
        void printTrajectory() const;
        
};


#endif
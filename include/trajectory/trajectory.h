/**
 * @file trajectory.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Provides an interface to send trajectories to WidowX robot arm
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H


#include "controller.h"
#include "trajectoryerror.h"

#define WAITING_TIME 10000

/**
 * @brief class Trajectory
 * 
 */
class Trajectory{

    private:
        Controller* device;
        std::vector<std::vector<uint16_t>*>* trajectories;
        // TODO: add time management (pauses during execution, varying speed of servomotors, ...)

        bool waitFeedback() const;
        void move(const std::vector<uint16_t>& point) const;

    public:
        Trajectory(Controller* toDevice); // Does not create the controller
        virtual ~Trajectory(); // Does not delete the controller to be able to reuse it to launch other trajectories

        void addPoint(const std::vector<uint16_t>& point, int pos);
        void addPoint(const std::vector<uint16_t>& point);
        void removePoint(int pos);
        void removePoint();

        void init() const;
        void executeTrajectory() const;
        void printTrajectory() const;
        
};


#endif

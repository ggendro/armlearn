/**
 * @file trajectory.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Provides an interface to send trajectories to WidowX robot arm
 * @version 0.1
 * @date 2019-05-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <iterator>
#include <thread>

#include "abstractcontroller.h"
#include "trajectoryerror.h"

namespace armlearn {

/**
 * @class Trajectory
 * @brief class Trajectory
 * 
 */
class Trajectory{

    private:
        communication::AbstractController* device;
        std::vector<std::vector<uint16_t>*>* trajectories;
        // TODO: add time management (pauses during execution, varying speed of servomotors, ...)
        

        /**
         * @brief Move to designed point
         * 
         * @param point the point to go to
         * @throw TrajectoryError if no feedback comes from the device after execution
         */
        void move(const std::vector<uint16_t>& point) const;

    public:

        /**
         * @brief Constructs a new Trajectory:: Trajectory object
         * 
         * @param toDevice pointer to the Controller device to send the trajectories to, does not create it
         */
        Trajectory(communication::AbstractController* toDevice);

        /**
         * @brief Destroys the Trajectory:: Trajectory object
         * 
         * Does not delete the controller to be able to reuse it to launch other trajectories
         */
        virtual ~Trajectory();


        /**
         * @brief Adds a point to the trajectory at a specified position
         * 
         * @param point the coordinates of the servomotors
         * @param pos the position in the trajectory to put the point
         */
        void addPoint(const std::vector<uint16_t>& point, int pos);

        /**
         * @brief Adds a point to the end of the trajectory
         * 
         * @param point the coordinates of the servomotors
         */
        void addPoint(const std::vector<uint16_t>& point);

        /**
         * @brief Removes a point from the trajectory
         * 
         * @param pos the position of the point to remove in the trajectory
         */
        void removePoint(int pos);

        /**
         * @brief Removes the last point from the trajectory
         * 
         */
        void removePoint();


        /**
         * @brief Initializes the position of the device to the first position stored
         * 
         * @throw TrajectoryError if no feedback comes from the device after execution
         */
        void init(bool reverse = false) const; // TODO: add more flexibility for pointer choice

        /**
         * @brief Executes the stored trajectory
         * 
         * @throw TrajectoryError if no feedback comes from the device after execution
         */
        void executeTrajectory(bool reverse = false) const;

        /**
         * @brief Prints the trajectory
         * 
         */
        void printTrajectory() const;
        
};

}

#endif

/**
 * @copyright Copyright (c) 2019
 */


#include "trajectory.h"


/**
 * @brief Construct a new Trajectory:: Trajectory object
 * 
 * @param toDevice pointer to the WidowX deviceto send the trajectories to
 */
Trajectory::Trajectory(WidowX* toDevice){
    device = toDevice;

    if(device->getMode() != backhoe) device->changeMode(backhoe);

    trajectories = new std::vector<std::vector<int>*>();
}

/**
 * @brief Destroy the Trajectory:: Trajectory object
 * 
 */
Trajectory::~Trajectory(){
    for(auto ptr1 = trajectories->begin(); ptr1 < trajectories->end(); ptr1++){
        delete *ptr1;
    }

    delete trajectories;
}


/**
 * @brief Wait for a device response, used for trajectory execution
 * 
 * @return true if a response occured within the allowed time
 * @return false otherwise
 */

bool Trajectory::waitFeedback() const{ // TODO: Really collect response, not only wait
    
    /*
    std::vector<int> buf;
    device->read(buf, true, WAITING_TIME);

    return buf.size() > 0;
    */

   std::this_thread::sleep_for((std::chrono::milliseconds) WAITING_TIME);
   return true;
}





/**
 * @brief Add a point to the trajectory at a specified position
 * 
 * @param point the coordinates of the servomotors
 * @param pos the position in the trajectory to put the point
 */
void Trajectory::addPoint(const std::vector<int>& point, int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");
    //if(!device->isMoveValid(point)) throw MovementError("Error : Movement not allowed by device");

    std::vector<int>* newPoint = new std::vector<int>(point.cbegin(), point.cend());

    std::vector<std::vector<int>*>::iterator ptrPos = trajectories->begin();
    ptrPos += pos;

    trajectories->insert(ptrPos, newPoint);
}

/**
 * @brief Add a point to the end of the trajectory
 * 
 * @param point the coordinates of the servomotors
 */
void Trajectory::addPoint(const std::vector<int>& point){
    addPoint(point, this->trajectories->size());
}

/**
 * @brief Remove a point from the trajectory
 * 
 * @param pos the position of the point to remove in the trajectory
 */
void Trajectory::removePoint(int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");

    std::vector<std::vector<int>*>::iterator ptrPos = trajectories->begin();
    ptrPos += pos;

    trajectories->erase(ptrPos);
}

/**
 * @brief Remove the last point from the trajectory
 * 
 */
void Trajectory::removePoint(){
    removePoint(this->trajectories->size());
}


/**
 * @brief Initialize the position of the device to the first position stored
 * 
 * @throw TrajectoryError if no feedback comes from the device after execution
 */
void Trajectory::init() const{
    if(device->getMode() != backhoe) device->changeMode(backhoe);

    try{
        device->move(**trajectories->cbegin());
    }catch(MovementError e){
        throw TrajectoryError(e.what());
    }
    
    if(!waitFeedback()) throw TrajectoryError("Timeout error during initialization : no response from the device");
    std::cout << "Initialized" << std::endl;
}

/**
 * @brief Executes the stored trajectory
 * 
 * @throw TrajectoryError if no feedback comes from the device after execution
 */
void Trajectory::executeTrajectory() const{
    for(auto ptr = trajectories->cbegin(); ptr < trajectories->cend(); ptr++){

        try{
            device->move(**ptr);
        }catch(MovementError e){
            throw TrajectoryError(e.what());
        }
        
        if(!waitFeedback()) throw TrajectoryError("Timeout error during execution : no response from the device");
        std::cout << "Move executed" << std::endl;	
    }
}

/**
 * @brief Print the trajectory
 * 
 */
void Trajectory::printTrajectory() const{
    std::cout << "Trajectory:" << std::endl;

    int i = 0;
    for(auto ptr1=trajectories->cbegin(); ptr1 < trajectories->cend(); ptr1++){
        std::cout << i << ".\t";
        i++;

        for(auto ptr2=(*ptr1)->cbegin(); ptr2 < (*ptr1)->cend(); ptr2++){
            std::cout << *ptr2 << "\t";
        }
        std::cout << std::endl;
    }

}
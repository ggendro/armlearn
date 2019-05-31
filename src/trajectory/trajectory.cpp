/**
 * @copyright Copyright (c) 2019
 */


#include "trajectory.h"


/**
 * @brief Construct a new Trajectory:: Trajectory object
 * 
 * @param toDevice pointer to the WidowX deviceto send the trajectories to
 */
Trajectory::Trajectory(Controller* toDevice){
    device = toDevice;

    trajectories = new std::vector<std::vector<uint16_t>*>();
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

bool Trajectory::waitFeedback() const{

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    bool response = device->goalReached();

    while(!response && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < WAITING_TIME){ // TODO: implement without active waiting
		std::this_thread::sleep_for((std::chrono::milliseconds) WAITING_TIME/10);
        device->updateInfos();
        response = device->goalReached();
        std::cout << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
        
        currentTime = std::chrono::system_clock::now();
	}

   return response;
}

/**
 * @brief Move to designed point
 * 
 * @param point the point to go to
 * @throw TrajectoryError if no feedback comes from the device after execution
 */
void Trajectory::move(const std::vector<uint16_t>& point) const{
    try{
        device->setPosition(point);
    }catch(ConnectionError e){
        throw TrajectoryError(e.what()); //TODO: handle instead of throwing
    }catch(OutOfRangeError e){
        throw TrajectoryError(e.what()); //TODO: handle instead of throwing
    }catch(IdError e){
        throw TrajectoryError(e.what()); //TODO: handle instead of throwing
    }

    waitFeedback();
}





/**
 * @brief Add a point to the trajectory at a specified position
 * 
 * @param point the coordinates of the servomotors
 * @param pos the position in the trajectory to put the point
 */
void Trajectory::addPoint(const std::vector<uint16_t>& point, int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");

    std::vector<uint16_t>* newPoint = new std::vector<uint16_t>(point.cbegin(), point.cend());
    std::vector<std::vector<uint16_t>*>::iterator ptrPos = trajectories->begin();
    ptrPos += pos;

    trajectories->insert(ptrPos, newPoint);
}

/**
 * @brief Add a point to the end of the trajectory
 * 
 * @param point the coordinates of the servomotors
 */
void Trajectory::addPoint(const std::vector<uint16_t>& point){
    addPoint(point, this->trajectories->size());
}

/**
 * @brief Remove a point from the trajectory
 * 
 * @param pos the position of the point to remove in the trajectory
 */
void Trajectory::removePoint(int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");

    std::vector<std::vector<uint16_t>*>::iterator ptrPos = trajectories->begin();
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
    move(**trajectories->cbegin());
}

/**
 * @brief Executes the stored trajectory
 * 
 * @throw TrajectoryError if no feedback comes from the device after execution
 */
void Trajectory::executeTrajectory() const{
    for(auto ptr = trajectories->cbegin(); ptr < trajectories->cend(); ptr++) move(**ptr);
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

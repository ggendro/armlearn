/**
 * @copyright Copyright (c) 2019
 */


#include "trajectory.h"


Trajectory::Trajectory(Controller* toDevice){
    device = toDevice;

    trajectories = new std::vector<std::vector<uint16_t>*>();
}

Trajectory::~Trajectory(){
    for(auto ptr1 = trajectories->begin(); ptr1 < trajectories->end(); ptr1++){
        delete *ptr1;
    }

    delete trajectories;
}


bool Trajectory::waitFeedback() const{

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

    device->updateInfos();
    bool response = device->goalReached();

    while(!response && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < WAITING_TIME){ // TODO: implement without active waiting
        std::cout << "Movement not entirely executed at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
        std::this_thread::sleep_for((std::chrono::milliseconds) WAITING_TIME/10);

        device->updateInfos();
        response = device->goalReached();

        if(!response){
            double sse = device->positionSumSquaredError();
            std::cout << "Sum of squared errors : " << sse << std::endl;

            if(sse < MARGIN_ERROR) response = true;
        }
        
        currentTime = std::chrono::system_clock::now();
        std::cout <<"Goal reached : " << response << " - New try at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
	}

   return response;
}

void Trajectory::move(const std::vector<uint16_t>& point) const{
    try{
        device->setPosition(point);
    }catch(ConnectionError e){
        throw TrajectoryError(e.what()); //TODO: handle instead of throwing
    }catch(OutOfRangeError e){
        throw TrajectoryError(e.what());
    }catch(IdError e){
        throw TrajectoryError(e.what());
    }

    waitFeedback();
}





void Trajectory::addPoint(const std::vector<uint16_t>& point, int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");

    std::vector<uint16_t>* newPoint = new std::vector<uint16_t>(point.cbegin(), point.cend());
    std::vector<std::vector<uint16_t>*>::iterator ptrPos = trajectories->begin();
    ptrPos += pos;

    trajectories->insert(ptrPos, newPoint);
}

void Trajectory::addPoint(const std::vector<uint16_t>& point){
    addPoint(point, this->trajectories->size());
}


void Trajectory::removePoint(int pos){
    if(pos <0 || pos > trajectories->size()) throw std::out_of_range("Error : Value out of vector boundaries");

    std::vector<std::vector<uint16_t>*>::iterator ptrPos = trajectories->begin();
    ptrPos += pos;

    trajectories->erase(ptrPos);
}

void Trajectory::removePoint(){
    removePoint(this->trajectories->size());
}


void Trajectory::init(bool reverse) const{
    if(reverse) 
        move(**trajectories->rbegin());
    else 
        move(**trajectories->cbegin());
}


void Trajectory::executeTrajectory(bool reverse) const{
    if(reverse)
        for(auto ptr = trajectories->rbegin(); ptr < trajectories->rend(); ptr++) move(**ptr);
    else
        for(auto ptr = trajectories->cbegin(); ptr < trajectories->cend(); ptr++) move(**ptr);
}


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

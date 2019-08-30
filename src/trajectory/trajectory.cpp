/**
 * @copyright Copyright (c) 2019
 */


#include "trajectory.h"

using namespace armlearn;


Trajectory::Trajectory(communication::AbstractController* toDevice){
    device = toDevice;

    trajectories = new std::vector<std::vector<uint16_t>*>();
}

Trajectory::~Trajectory(){
    for(auto ptr1 = trajectories->begin(); ptr1 < trajectories->end(); ptr1++){
        delete *ptr1;
    }

    delete trajectories;
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

    device->waitFeedback();
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
    if(pos <0 || pos > trajectories->size()-1) throw std::out_of_range("Error : Value out of vector boundaries");

    trajectories->erase(trajectories->begin()+pos);
}

void Trajectory::removePoint(){
    removePoint(this->trajectories->size()-1);
}


void Trajectory::init(bool reverse) const{
    if(trajectories->size() == 0) return;

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

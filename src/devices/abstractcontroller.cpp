/**
 * @copyright Copyright (c) 2019
 */

#include "abstractcontroller.h"

AbstractController::AbstractController(DisplayMode displayMode, std::ostream& out):mode(displayMode), output(out) {
    motors = new std::map<uint8_t, Servomotor*>();
}

AbstractController::~AbstractController(){
    for(auto ptr = motors->begin(); ptr != motors->end(); ptr++){
        delete ptr->second;
    }
    delete motors;
}


DisplayMode AbstractController::getDisplayMode() const{
    return mode;
}

void AbstractController::setDisplayMode(DisplayMode newMode){
    mode = newMode;
}


bool AbstractController::addMotor(uint8_t id, const std::string& name, Type type){
    if(motors->find(id) != motors->end()){
        if(mode >= except) throw IdError("ID already taken.");
        return false;
    } 

    Servomotor* motor = new Servomotor(id, name, type);
    motors->insert(std::pair<int, Servomotor*>(id, motor));

    return true;
}

bool AbstractController::removeMotor(uint8_t id){
    auto ptr = motors->find(id);
    if(ptr == motors->end()){
        if(mode >= except) throw IdError("ID not found.");
        return false;
    } 

    delete ptr->second;
    motors->erase(ptr);

    return true;
}


void AbstractController::changeSpeed(uint16_t newSpeed){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){ 
        changeSpeed(ptr->first, newSpeed);
    }
}


void AbstractController::setPosition(const std::vector<uint16_t>& newPosition){
    auto ptrPos = newPosition.cbegin();
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){ 
        setPosition(ptr->first, *ptrPos);
        ptrPos++;
    }
}

void AbstractController::goToBackhoe(){
    setPosition(BACKHOE_POSITION);
}

void AbstractController::goToSleep(){
    setPosition(SLEEP_POSITION);
}


void AbstractController::addPosition(const std::vector<int> dx){
    auto ptrPos = dx.cbegin();
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){  
        addPosition(ptr->first, *ptrPos);
        ptrPos++;
    }
}


std::vector<uint16_t> Controller::getPosition() const{
    std::vector<uint16_t> res;
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){  
        res.push_back(ptr->second->getCurrentPosition());
    }

    return res;
}


bool Controller::goalReached() const{
    for(auto ptr=motors->cbegin(); ptr != motors->cend(); ptr++){
        if(ptr->second->motorMoving()) return false;
    }

    return true;
}

double Controller::positionSumSquaredError() const{
    double sse = 0;
    for(auto ptr=motors->cbegin(); ptr != motors->cend(); ptr++){
        sse += std::pow(ptr->second->targetPositionReached(), 2);
    }

    return std::sqrt(sse);
}


void Controller::updateInfos(){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){
        updateInfos(ptr->first);
    }
    if(mode >= print) output << servosToString();
}


std::string Controller::servosToString() const {
    std::stringstream streamRep;
    streamRep << "Servomotors :" << std::endl;
    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++) streamRep << ptr->second->toString() << std::endl;

    return streamRep.str();
}
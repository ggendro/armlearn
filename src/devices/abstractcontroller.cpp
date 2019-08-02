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
    for(auto ptr=motors->begin(); ptr != motors->end() && ptrPos != newPosition.cend(); ptr++){ 
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


void AbstractController::addPosition(const std::vector<int>& dx){
    auto ptrPos = dx.cbegin();
    for(auto ptr=motors->begin(); ptr != motors->end() && ptrPos != dx.end(); ptr++){  
        addPosition(ptr->first, *ptrPos);
        ptrPos++;
    }
}


std::vector<uint16_t> AbstractController::getPosition() const{
    std::vector<uint16_t> res;
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){  
        res.push_back(ptr->second->getCurrentPosition());
    }

    return res;
}

bool AbstractController::validPosition(const std::vector<uint16_t>& position) const{
    if(position.size() < motors->size()) return false;

    auto posPtr = position.cbegin(); 
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){  
        if(!ptr->second->validPosition(*posPtr)) return false;
        posPtr++;
    }

    return true;
}


bool AbstractController::goalReached() const{
    for(auto ptr=motors->cbegin(); ptr != motors->cend(); ptr++){
        if(ptr->second->motorMoving()) return false;
    }

    return true;
}

double AbstractController::positionSumSquaredError() const{
    double sse = 0;
    for(auto ptr=motors->cbegin(); ptr != motors->cend(); ptr++){
        sse += std::pow(ptr->second->targetPositionReached(), 2);
    }

    return std::sqrt(sse);
}

bool AbstractController::waitFeedback(int sleepTime, int allowedTime){

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

    updateInfos();
    bool response = goalReached();

    while(!response && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < allowedTime){ // TODO: implement without active waiting
        if(mode >= print) output << "Movement not entirely executed at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
        std::this_thread::sleep_for((std::chrono::milliseconds) sleepTime);

        updateInfos();
        response = goalReached();

        if(!response){
            double sse = positionSumSquaredError();
            if(mode >= print) output << "Sum of squared errors : " << sse << std::endl;

            if(sse < POSITION_ERROR_MARGIN) response = true;
        }
        
        currentTime = std::chrono::system_clock::now();
        if(mode >= print) output <<"Goal reached : " << response << " - New try at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
	}

   return response;
}


void AbstractController::updateInfos(){
    for(auto ptr=motors->begin(); ptr != motors->end(); ptr++){
        updateInfos(ptr->first);
    }
    if(mode >= print) output << servosToString();
}


std::string AbstractController::servosToString() const {
    std::stringstream streamRep;
    streamRep << "Servomotors :" << std::endl;
    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++) streamRep << ptr->second->toString() << std::endl;

    return streamRep.str();
}
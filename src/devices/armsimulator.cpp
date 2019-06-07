/**
 * @copyright Copyright (c) 2019
 */


#include "armsimulator.h"

ArmSimulator::ArmSimulator(DisplayMode displayMode, std::ostream& out):AbstractController(displayMode, out){

}

ArmSimulator::~ArmSimulator(){

}


bool ArmSimulator::getMotor(uint8_t id, Servomotor*& ptr){
    auto it = motors->find(id);
    if(it == motors->end()){
        std::stringstream disp;
        disp << "ID " << (int) id <<" not found.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw IdError(disp.str());
        return false;
    }
    ptr = it->second;
    return true;
}



void ArmSimulator::connect(){

    for(auto ptr = motors->cbegin(); ptr != motors->cend(); ptr++){
        ptr->second->setStatus(activated);
    }

}

void ArmSimulator::ping(uint8_t id){
    if(mode > none)
        output << "Ping " << id << std::endl;
}


bool ArmSimulator::changeId(uint8_t oldId, uint8_t newId){
    Servomotor* oldPtr;
    Servomotor* newPtr;

    bool res = getMotor(oldId, oldPtr) && !getMotor(newId, newPtr);
    if(!res) return false;

    oldPtr->setId(newId); // Change in the servo class

    motors->erase(motors->find(oldId)); // Change in the list
    motors->insert(std::pair<int, Servomotor*>(newId, oldPtr));

    return true;
}

bool ArmSimulator::turnLED(uint8_t id, bool on){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    ptr->setLED(on);
    return true;
}

bool ArmSimulator::turnLED(uint8_t id){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    ptr->setLED(!ptr->getLED());
    return true;
}


bool ArmSimulator::changeSpeed(uint8_t id, uint16_t newSpeed){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    if(!ptr->validSpeed(newSpeed)){
        std::stringstream disp;
        disp << "Speed value " << newSpeed << " is out of the range.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw OutOfRangeError(disp.str());
        return 0; 
    }

    ptr->setTargetSpeed(newSpeed);
    return true;
}


bool ArmSimulator::setPosition(uint8_t id, uint16_t newPosition){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    if(!ptr->validPosition(newPosition)){
        std::stringstream disp;
        disp << "Position " << newPosition <<" is out of the range.";

        if(mode >= print) output << disp.str() << std::endl;
        if(mode >= except) throw OutOfRangeError(disp.str());
        
        return 0;
    }

    ptr->setTargetPosition(newPosition);
    return true;
}

bool ArmSimulator::addPosition(uint8_t id, int dx){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    return setPosition(id, ptr->getTargetPosition() + dx);
}


bool ArmSimulator::enableTorque(int id, bool enable){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    ptr->setStatus(enable ? activated : connected);
}

bool ArmSimulator::torqueEnabled(int id){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    return ptr->getStatus() > connected;
}


bool ArmSimulator::updateInfos(uint8_t id){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    // Computation of distance reached since last update
    uint16_t dest =  ptr->getTargetPosition();
    uint16_t start =  ptr->getCurrentPosition();
    uint16_t spd =  ptr->getTargetSpeed();
    double time = ptr->getTimeSinceUpdate();
    bool moving = true;
    uint16_t distReached = spd * SPEED_UNIT * time / 60 / M_PI * 180 / MOVE_UNIT;

    // If destination is already reached
    if(std::abs(dest-start) < distReached){
        distReached = dest;
        spd = 0;
        moving = false;
    } 

    ptr->setInfos({(uint8_t) distReached, (uint8_t) (distReached >> BYTE_SIZE), (uint8_t) spd, (uint8_t) (spd >> BYTE_SIZE), (uint8_t) CURRENT_LOAD, (uint8_t) (CURRENT_LOAD >> BYTE_SIZE), CURRENT_VOLTAGE, CURRENT_TEMP, INSTRUCTION_WAITING, 0, (uint8_t) moving});
    return true;
}
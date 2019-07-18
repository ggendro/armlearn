/**
 * @copyright Copyright (c) 2019
 */


#include "nowaitarmsimulator.h"

NoWaitArmSimulator::NoWaitArmSimulator(DisplayMode displayMode, std::ostream& out):ArmSimulator(displayMode, out){

}

NoWaitArmSimulator::~NoWaitArmSimulator(){

}


bool NoWaitArmSimulator::updateInfos(uint8_t id){
    Servomotor* ptr;
    if(!getMotor(id, ptr)) return false;

    // Computation of distance reached since last update
    uint16_t distReached =  ptr->getTargetPosition();
    uint16_t spd = 0;
    bool moving = false;

    ptr->setInfos({(uint8_t) distReached, (uint8_t) (distReached >> BYTE_SIZE), (uint8_t) spd, (uint8_t) (spd >> BYTE_SIZE), (uint8_t) CURRENT_LOAD, (uint8_t) (CURRENT_LOAD >> BYTE_SIZE), CURRENT_VOLTAGE, CURRENT_TEMP, INSTRUCTION_WAITING, 0, (uint8_t) moving});
    return true;
}


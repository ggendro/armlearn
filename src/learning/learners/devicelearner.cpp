/**
 * @copyright Copyright (c) 2019
 */


#include "devicelearner.h"


DeviceLearner::DeviceLearner(AbstractController* controller, double testProp):Learner(testProp){
    device = controller;
}


std::vector<std::vector<uint16_t>> DeviceLearner::getDeviceState() const{
    std::vector<std::vector<uint16_t>> motorsState;
    for(auto ptr = device->motors->begin(); ptr != device->motors->end(); ptr++){
        std::vector<uint16_t> vectMotor;

        vectMotor.push_back((uint16_t) ptr->second->id);
        vectMotor.push_back((uint16_t) ptr->second->status);
        vectMotor.push_back((uint16_t) ptr->second->type);

        vectMotor.push_back((uint16_t) ptr->second->posMin);
        vectMotor.push_back((uint16_t) ptr->second->posMax);

        vectMotor.push_back((uint16_t) ptr->second->modelNum);
        vectMotor.push_back((uint16_t) ptr->second->firmware);

        vectMotor.push_back((uint16_t) ptr->second->targetSpeed);
        vectMotor.push_back((uint16_t) ptr->second->targetPosition);


        vectMotor.push_back((uint16_t) ptr->second->position);
        vectMotor.push_back((uint16_t) ptr->second->speed);
        vectMotor.push_back((uint16_t) ptr->second->load);
        vectMotor.push_back((uint16_t) ptr->second->voltage);
        vectMotor.push_back((uint16_t) ptr->second->temperature);


        vectMotor.push_back((uint16_t) ptr->second->instructionRegistered);
        vectMotor.push_back((uint16_t) ptr->second->inMovement);

        vectMotor.push_back((uint16_t) ptr->second->activeLED);

        motorsState.push_back(vectMotor);
    }

    return motorsState;
}


template<class T> std::vector<T> DeviceLearner::getClosestValidPosition(std::vector<T> position, T securityThreshold) const{
    std::vector<T> correction;

    auto ptrPos = position.cbegin();
    for(auto ptrDev=device->motors->cbegin(); ptrDev != device->motors->cend(); ptrDev++){
        T newVal;
        if(ptrPos == position.cend()){
            newVal = ptrDev->second->posMin + securityThreshold;
        }else {
            if(ptrDev->second->validPosition(*ptrPos)){
                newVal = *ptrPos;
            }else{
                newVal = (*ptrPos < ptrDev->second->posMin) ? (ptrDev->second->posMin + securityThreshold) : (ptrDev->second->posMax - securityThreshold);
            }
            ptrPos++;
        }

        correction.push_back(newVal);
    }

    return correction;
}
template std::vector<uint16_t> DeviceLearner::getClosestValidPosition<uint16_t>(std::vector<uint16_t> position, uint16_t securityThreshold) const;
template std::vector<double> DeviceLearner::getClosestValidPosition<double>(std::vector<double> position, double securityThreshold) const;


std::string DeviceLearner::toString() const{
    std::stringstream rep;
    rep << "Device " << Learner::toString();
    rep << "Connected to " << device->motors->size() << " devices" << std::endl;

    return rep.str();
}
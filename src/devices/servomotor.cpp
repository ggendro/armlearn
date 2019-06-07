/**
 * @copyright Copyright (c) 2019
 */

#include "servomotor.h"

Servomotor::Servomotor(uint8_t id, const std::string& name, Type type):status(offline), posMin(0), posMax(0), targetSpeed(0), targetPosition(0), modelNum(0), firmware(0), speed(0), position(0), load(0), voltage(0), temperature(0), activeLED(0), instructionRegistered(0), inMovement(0){
    this->id = id;
    this->name = name;

    this->setType(type);

    auto currentTime = std::chrono::system_clock::now();
    creationTime = currentTime;
    lastUpdate = currentTime;
}

Servomotor::~Servomotor(){

}


uint8_t Servomotor::getId() const{
    return id;
}

std::string Servomotor::getName() const{
    return name;
}

Status Servomotor::getStatus() const{
    return status;
}

Type Servomotor::getType() const{
    return type;
}

bool Servomotor::getLED() const{
    return activeLED;
}

uint16_t Servomotor::getTargetSpeed() const{
    return targetSpeed;
}

uint16_t Servomotor::getTargetPosition() const{
    return targetPosition;
}

uint16_t Servomotor::getCurrentPosition() const{
    return position;
}

double Servomotor::getTimeSinceUpdate() const{
    return std::chrono::duration<double, std::ratio<1, 1>>(lastUpdate - creationTime).count();
}


std::string Servomotor::toString() const{
    std::stringstream streamRep;
    streamRep << "Servomotor " << name << "\tID: " << (int) id << "\tModel: " << (int) modelNum << "\tFirmware: " << (int) firmware << "\tStatus: " << status; // Model and status information
    streamRep << "\tLED: " << activeLED << "\tPosition: " << (int) position << " (target: " << targetPosition <<  ")" << "\tSpeed: " << (int) speed << " (target: " << targetSpeed <<  ")";
    streamRep << "\tLoad: " << (int) load << "\tVoltage: " << (int) voltage << "\tTemperature: " << (int) temperature; // Integer information
    streamRep << "\tInstruction waiting? " << instructionRegistered << "\tIn movement? " << inMovement; // Boolean information
    streamRep << "\tUpdated: " << std::chrono::duration<double, std::ratio<1, 1>>(std::chrono::system_clock::now() - lastUpdate).count() << "s ago "; // Time passed since last update was executed
    
    return streamRep.str();
}



void Servomotor::setName(const std::string& newName){
    name = newName;
}

void Servomotor::setStatus(Status stat){
    status = stat;
}

void Servomotor::setType(Type newType){
    type = newType;

    switch(newType){
        case base:
            posMin = BASE_MIN;
            posMax = BASE_MAX;
            break;

        case shoulder:
            posMin = SHOULDER_MIN;
            posMax = SHOULDER_MAX;
            break;

        case elbow:
            posMin = ELBOW_MIN;
            posMax = ELBOW_MAX;
            break;

        case wristAngle:
            posMin = WRISTANGLE_MIN;
            posMax = WRISTANGLE_MAX;
            break;

        case wristRotate:
            posMin = WRISTROTATE_MIN;
            posMax = WRISTROTATE_MAX;
            break;

        case gripper:
            posMin = GRIPPER_MIN;
            posMax = GRIPPER_MAX;
            break;

        default:
            throw TypeError("Type is not part of the list of types.");
            break;
    }
}

void Servomotor::setModel(uint16_t mod){
    modelNum = mod;
}

void Servomotor::setFirmware(uint8_t firm){
    firmware = firm;
}

void Servomotor::setId(uint8_t id){
    this->id = id;
}

void Servomotor::setLED(bool on){
    activeLED = on;
}

void Servomotor::setInfos(const std::vector<uint8_t>& infos){
    if(infos.size() != READ_LENGTH) return;

    position = infos[0] + (infos[1] << BYTE_SIZE);
    speed = infos[2] + (infos[3] << BYTE_SIZE);
    load = infos[4] + (infos[5] << BYTE_SIZE);
    voltage = infos[6];
    temperature = infos[7];
    instructionRegistered = infos[8];
    inMovement = infos[10];

    lastUpdate = std::chrono::system_clock::now();
}

void Servomotor::setTargetSpeed(uint16_t speed){
    targetSpeed = speed;
}

void Servomotor::setTargetPosition(uint16_t position){
    targetPosition = position;
}



bool Servomotor::validSpeed(uint16_t speed) const{
    return speed > SPEED_MIN && speed < SPEED_MAX;
}

bool Servomotor::validPosition(uint16_t position) const{
    return position > posMin && position < posMax;
}

bool Servomotor::motorMoving() const{
    return inMovement;
}


int Servomotor::targetPositionReached() const{
    return abs(targetPosition - position) + (inMovement ? ERROR_MOVING : 0);
}
/**
 * @copyright Copyright (c) 2019
 */

#include "servomotor.h"

/**
 * @brief Construct a new Servomotor:: Servomotor object
 * 
 * @param id the id of the servomotor
 * @param name the name of theservomotor
 */
Servomotor::Servomotor(uint8_t id, const std::string& name, Type type):status(offline), posMin(0), posMax(0), targetSpeed(0), targetPosition(0), modelNum(0), firmware(0), speed(0), position(0), load(0), voltage(0), temperature(0), activeLED(0), instructionregistered(0), inMovement(0){
    this->id = id;
    this->name = name;

    this->setType(type);

    auto currentTime = std::chrono::system_clock::now();
    creationTime = currentTime;
    lastUpdate = currentTime;
}

/**
 * @brief Destroy the Servomotor:: Servomotor object
 * 
 */
Servomotor::~Servomotor(){

}


/**
 * @brief Return the id of the servomotor
 * 
 * @return int the id with int format
 */
uint8_t Servomotor::getId() const{
    return id;
}

/**
 * @brief Return the name of the servomotor
 * 
 * @return std::string the name with string format
 */
std::string Servomotor::getName() const{
    return name;
}

/**
 * @brief Return the current status of the servomotor (connected or not)
 * 
 * @return Status an enum corresponding to the status of the servomotor device
 */
Status Servomotor::getStatus() const{
    return status;
}

/**
 * @brief Return the type of the servomotor
 * 
 * @return Type an enum corresponding to the type of the servomotor
 */
Type Servomotor::getType() const{
    return type;
}

/**
 * @brief Return the current state of the servomotor's LED
 * 
 * @return true if ON
 * @return false if OFF
 */
bool Servomotor::getLED() const{
    return activeLED;
}

/**
 * @brief Return the current target position
 * 
 * @return uint16_t the position to reach
 */
uint16_t Servomotor::getTargetPosition() const{
    return targetPosition;
}

/**
 * @brief Retunr a string containing informations about the servomotor (id, name, status, position, ...)
 * 
 * @return std::string informations contained in the servomotor under string format
 */
std::string Servomotor::toString() const{
    std::stringstream streamRep;
    streamRep << "Servomotor " << name << "\tID: " << (int) id << "\tModel: " << (int) modelNum << "\tFirmware: " << (int) firmware << "\tStatus: " << status; // Model and status information
    streamRep << "\tLED: " << activeLED << "\tPosition: " << (int) position << " (target: " << targetPosition <<  ")" << "\tSpeed: " << (int) speed << " (target: " << targetSpeed <<  ")";
    streamRep << "\tLoad: " << (int) load << "\tVoltage: " << (int) voltage << "\tTemperature: " << (int) temperature; // Integer information
    streamRep << "\tInstruction waiting? " << instructionregistered << "\tIn movement? " << inMovement; // Boolean information
    streamRep << "\tUpdated: " << std::chrono::duration<double, std::ratio<1, 1>>(std::chrono::system_clock::now() - lastUpdate).count() << "s ago "; // Time passed since last update was executed
    
    return streamRep.str();
}


/**
 * @brief Set a new status for the servomotor
 * 
 * @param status the new status
 */
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

/**
 * @brief Set model number of the servomotor
 * 
 * @param mod the model number to set
 */
void Servomotor::setModel(uint16_t mod){
    modelNum = mod;
}

/**
 * @brief Set firmware version of the servomotor
 * 
 * @param firm the firmware version to set
 */
void Servomotor::setFirmware(uint8_t firm){
    firmware = firm;
}

/**
 * @brief Set the id of the servomotor
 * 
 * @param id the new id to set
 */
void Servomotor::setId(uint8_t id){
    this->id = id;
}

/**
 * @brief Change the name of the servo
 * 
 * @param newName the new name
 */
void Servomotor::setName(const std::string& newName){
    name = newName;
}

/**
 * @brief Change the value of the LED
 * 
 * @param on the value of the LED (true = ON, false = OFF)
 */
void Servomotor::setLED(bool on){
    activeLED = on;
}

/**
 * @brief Set read-only informations of the servomotor
 * 
 * @param infos the infos to set
 * 
 * Informations set:
 *  - Present position
 *  - Current speed
 *  - Current load
 *  - Current voltage
 *  - Current temperature
 *  - Is an instruction registered in the servo
 *  - Is the servo currently in movement
 */
void Servomotor::setInfos(const std::vector<uint8_t>& infos){
    if(infos.size() != 10) return;

    position = infos[0] + (infos[1] << BYTE_SIZE);
    speed = infos[2] + (infos[3] << BYTE_SIZE);
    load = infos[4] + (infos[5] << BYTE_SIZE);
    voltage = infos[6];
    temperature = infos[7];
    instructionregistered = infos[8];
    inMovement = infos[9];

    lastUpdate = std::chrono::system_clock::now();
}

/**
 * @brief Set the target speed of the servomotor
 * 
 * @param speed the new speed to reach
 */
void Servomotor::setTargetSpeed(uint16_t speed){
    targetSpeed = speed;
}

/**
 * @brief Set the target position of the servomotor
 * 
 * @param speed the new position to reach
 */
void Servomotor::setTargetPosition(uint16_t position){
    targetPosition = position;
}


/**
 * @brief Check if the speed is within the boundaries of the servomotor
 * 
 * @return true if it is
 * @return false otherwise
 */
bool Servomotor::validSpeed(uint16_t speed) const{
    return speed > SPEED_MIN && speed < SPEED_MAX;
}

bool Servomotor::validPosition(uint16_t position) const{
    return position > posMin && position < posMax;
}

/**
 * @brief Check if the target position has been reached
 * 
 * @param err the margin of error, if negative, will use the default value
 * @return true if the difference between the target position and the position is inferior to the margin
 * @return false otherwise
 */
bool Servomotor::targetPositionReached(int errorRange) const{
    return abs(targetPosition - position) < errorRange;
}
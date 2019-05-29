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
Servomotor::Servomotor(uint8_t id, const std::string& name):modelNum(0), firmware(0), speed(0), position(0), load(0), voltage(0), temperature(0), activeLED(0), instructionregistered(0), inMovement(0){
    this->id = id;
    this->name = name;
    this->status = notConnected;

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
 * @brief Return the current stae of the servomotor's LED
 * 
 * @return true if ON
 * @return false if OFF
 */
bool Servomotor::getLED() const{
    return activeLED;
}

/**
 * @brief Retunr a string containing informations about the servomotor (id, name, status, position, ...)
 * 
 * @return std::string informations contained in the servomotor under string format
 */
std::string Servomotor::toString() const{
    std::stringstream streamRep;
    streamRep << "Servomotor " << name << "\tID: " << (int) id << "\tModel: " << (int) modelNum << "\tFirm: " << (int) firmware << "\tStat: " << status; // Model and status information
    streamRep << "\tLED: " << activeLED << "\tPos: " << position << "\tSpd: " << (int) speed << "\tLoad: " << (int) load << "\tVolt: " << (int) voltage << "\tTemp: " << (int) temperature; // Integer information
    streamRep  << "\tInstr? " << instructionregistered << "\tMove? " << inMovement; // Boolean information
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
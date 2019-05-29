/**
 * @file servomotor.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the servomotor class
 * @version 0.1
 * @date 2019-05-28
 * 
 * Documentation about the servomotor registers can be found at http://support.robotis.com/en/product/actuator/dynamixel/mx_series/mx-64at_ar.htm
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <vector>
#include <string>
#include <sstream>
#include <chrono>

// Starting address of the register containing the model number
#define MODEL_REGISTER 0x00
// Number of successive registers containging information about model number, firmware version and ID
#define MODEL_LENGTH 0x04 

// Starting address of the register containing the read-only information about the state of the servomotor
#define READ_REGISTER 0x24
// Number of successive registers containging read-only information
#define READ_LENGTH 0x0A

// Address of the register containing the ID of the servomotor
#define ID_REGISTER 0x03


enum Status{
    notConnected,
    connected
};


/**
 * @class Servomotor
 * @brief Represents a servomotor
 * 
 */
class Servomotor{

    private:
        uint8_t id;
        std::string name;
        Status status;

        uint16_t modelNum;
        uint8_t firmware;

        uint16_t position;
        uint16_t speed;
        uint16_t load;
        uint8_t voltage;
        uint8_t temperature;

        bool instructionregistered;
        bool inMovement;

        bool activeLED;

        std::chrono::time_point<std::chrono::system_clock> creationTime;
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        

    public:
        Servomotor(uint8_t id, const std::string& name);
        ~Servomotor();

        uint8_t getId() const;
        std::string getName() const;
        Status getStatus() const;
        std::string toString() const;

        void setStatus(Status stat);
        void setModel(uint16_t mod);
        void setFirmware(uint8_t firm);
        void setId(uint8_t id);
        void setInfos(const std::vector<uint8_t>& infos);

};

#endif
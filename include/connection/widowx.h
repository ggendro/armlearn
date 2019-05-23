/**
 * @file widowx.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Provides an interface to communicate with WidowX robot arm using serial port
 * @version 0.1
 * @date 2019-05-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef WIDOWX_H
#define WIDOWX_H

#include <serial/serial.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "definitions.h"
#include "connectionerror.h"
#include "movementerror.h"
#include "outofrangeerror.h"
#include "range.h"

/**
 * @brief WidowX class
 * 
 */
class WidowX{

    private:
        std::vector<Range*>* ranges; //TODO: (mode management) modify data structure to be able to check other modes as well
        serial::Serial* serialPort;
        Mode mode;

        uint16_t positions[NB_BIG_VALUE_MOTORS];
        // positions[0] - base
        // positions[1] - shoulder;
        // positions[2] - elbow;
        // positions[3] - wristAngle;
        // positions[4] - wristRotation;
        // positions[5] - gripper;

        uint8_t delta;


        inline void open();
        void setPositions(const std::vector<uint16_t>& values);
        void setDelta(const uint8_t value);

        bool connect();
        uint8_t computeChecksum(const std::vector<uint8_t>& data);
        bool checkValidity(const std::vector<uint8_t>& data);

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer);
        void forceMove(const std::vector<uint16_t>& positions); // Move without verifications


    public:
        WidowX(const std::string port); // Can throw exceptions if unable to connect
        virtual ~WidowX();

        void move(const std::vector<int>& positions); // 6 positions to fill, one for each servomotor, can throw exceptions if incorrect values
        bool isMoveEnabled() const;
        bool isMoveValid(const std::vector<int>& positions) const;

        void changeSpeed(int newSpeed); // Can throw exceptions if incorrect value
        void changeMode(Mode newMode);

        void read(std::vector<uint8_t>& buffer, bool wait=false, int timeout = 10000);

        static Mode stringToMode(const std::string& mode);
        static std::string modeToString(const Mode mode);

};

#endif
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


#include <serial/serial.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include "definitions.h"

/**
 * @brief WidowX class
 * 
 */
class WidowX{

    private:
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
        bool isMoveEnabled();

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer);
        void forceMove(const std::vector<uint16_t>& positions); // Move without verifications


    public:
        WidowX(const std::string port);
        virtual ~WidowX();

        void move(const std::vector<int>& positions); // 6 positions to fill, one for each servomotor
        void changeSpeed(int newSpeed);
        void changeMode(Mode newMode);

};




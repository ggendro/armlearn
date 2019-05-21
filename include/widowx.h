
#include <serial/serial.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#define HEADER 255
#define ENABLE_OUTPUT 0
#define CARTESIAN_MODE 32
#define BACKHOE_MODE 64
#define SLEEP_MODE 96
#define MOVE_MODE 0
#define CONNECT_REQUEST 112

// maximum delay to attempt to connect to the board (in ms)
#define CONNECT_TIMEOUT 10000

#define NB_BIG_VALUE_MOTORS 6
#define NB_SMALL_VALUE_MOTORS 1

#define CARTESIAN_POSITION {2048, 2048, 2048, 2048, 512, 159}
#define BACKHOE_POSITION {2048, 2048, 2048, 2048, 512, 159} //WRONG
#define SLEEP_POSITION {2048, 2048, 2048, 2048, 512, 159} //wrong too obviously
#define DEFAULT_SPEED 205

class WidowX{

    private:
        serial::Serial* serialPort;
        int id;

        uint16_t positions[NB_BIG_VALUE_MOTORS];
        // positions[0] - base
        // positions[1] - shoulder;
        // positions[2] - elbow;
        // positions[3] - wristAngle;
        // positions[4] - wristRotation;
        // positions[5] - gripper;

        uint8_t delta;


        inline void open();
        void setPositions(uint16_t* values);
        void setDelta(uint8_t value);

        bool connect();
        uint8_t computeChecksum(const std::vector<uint8_t>& data);
        bool checkValidity(const std::vector<uint8_t>& data);

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer);


    public:
        WidowX(const std::string port);
        virtual ~WidowX();

        void move(const std::vector<int>& positions); // 6 positions to fill, one for each motor
        void changeSpeed(int newSpeed);
        void goToBackhoe();
        void goToCartesian();
        void goToSleep();

};


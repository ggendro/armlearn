
#include <serial/serial.h>

#define HEADER 255
#define ENABLE_OUTPUT 0
#define BACKHOE_MODE 64
#define SLEEP_MODE 96
#define MOVE_MODE 0

#define NB_BIG_VALUE_MOTORS 6
#define NB_SMALL_VALUE_MOTORS 1

#define BACKHOE_POSITION {2048, 2048, 2048, 2048, 512, 159}
#define SLEEP_POSITION {2048, 2048, 2048, 2048, 512, 159}

class WidowX{

    private:
        serial::Serial* serialPort;

        uint16_t positions[6];
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

        uint8_t computeChecksum(const std::vector<uint8_t>& data);

        int send(const std::vector<uint8_t>& buffer);
        int receive(std::vector<uint8_t>& buffer);


    public:
        WidowX(const std::string port);
        virtual ~WidowX();

        void move(const std::vector<int>& positions);
        void changeSpeed(int newSpeed);
        void goToBackhoe();
        void goToSleep();


};


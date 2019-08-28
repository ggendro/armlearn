

#include "serialcontroller.h"

using namespace armlearn;

int main(int argc, char *argv[]) {

    /******************************************/
    /****     Common base for examples     ****/
    /******************************************/

    communication::SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", communication::base);
	arbotix.addMotor(2, "shoulder", communication::shoulder);
	arbotix.addMotor(3, "elbow", communication::elbow);
	arbotix.addMotor(4, "wristAngle", communication::wristAngle);
	arbotix.addMotor(5, "wristRotate", communication::wristRotate);
	arbotix.addMotor(6, "gripper", communication::gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();


	std::this_thread::sleep_for((std::chrono::milliseconds) 1000); // Usually, a waiting period and a second connect attempt is necessary to reach all devices
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50); // Servomotor speed is reduced for safety

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


    /******************************************/
    /****            LED example           ****/
    /******************************************/

    /*
     * Turn on LED of servomotor with ID 2, usually the shoulder servomotor. Wait 10 seconds and turn it off. 
     * 
     */

    int id = 2;

    std::cout << "Turn ON LED of servomotor "  << id << "." << std::endl;
	arbotix.turnLED(id);
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 10000);

	std::cout << "Turn OFF LED of servomotor  " << id << "." << std::endl;
	arbotix.turnLED(id);
	std::cout << arbotix.servosToString();

}
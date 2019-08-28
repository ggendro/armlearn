

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
    /****             ID example           ****/
    /******************************************/

    /*
     * Change ID of servomotor 6 to 7 and turn it back.
     * 
     */

    int oldId = 6;
    int newId = 7;

    std::cout << "Change ID servomotor " << oldId << " to " << newId << "." << std::endl;
	arbotix.changeId(6, 7);
	std::cout << arbotix.servosToString();

	std::cout << "Change ID servomotor " << newId << " to " << oldId << "." << std::endl;
	arbotix.changeId(7, 6);
	std::cout << arbotix.servosToString();

}
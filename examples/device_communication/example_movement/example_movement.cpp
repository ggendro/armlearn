

#include "serialcontroller.h"

int main(int argc, char *argv[]) {

    /******************************************/
    /****     Common base for examples     ****/
    /******************************************/

    SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", base);
	arbotix.addMotor(2, "shoulder", shoulder);
	arbotix.addMotor(3, "elbow", elbow);
	arbotix.addMotor(4, "wristAngle", wristAngle);
	arbotix.addMotor(5, "wristRotate", wristRotate);
	arbotix.addMotor(6, "gripper", gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();


	std::this_thread::sleep_for((std::chrono::milliseconds) 1000); // Usually, a waiting period and a second connect attempt is necessary to reach all devices
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50); // Servomotor speed is reduced for safety

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


    /******************************************/
    /****         Movement example         ****/
    /******************************************/

    /*
     * Set the arm to backhoe position, add some values to its coordinates and set it back to sleep position.
     * 
     */

    std::cout << "Set backhoe position:" << std::endl;
	arbotix.goToBackhoe();

	while(!arbotix.goalReached()){
		std::cout << "Goal not reached yet, wait..." << std::endl;
		arbotix.updateInfos();
		std::this_thread::sleep_for((std::chrono::milliseconds) 100);
	}
	

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();

	std::cout << "Add to position:" << std::endl;
	arbotix.addPosition({100, 50, 50, 50, -50, -50});

	while(!arbotix.goalReached()){
		std::cout << "Goal not reached yet, wait..." << std::endl;
		arbotix.updateInfos();
		std::this_thread::sleep_for((std::chrono::milliseconds) 100);
	}

	std::cout << "Set sleep position:" << std::endl;
	arbotix.goToSleep();

	while(!arbotix.goalReached()){
		std::cout << "Goal not reached yet, wait..." << std::endl;
		arbotix.updateInfos();
		std::this_thread::sleep_for((std::chrono::milliseconds) 100);
	}


	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();

}



#include "controller.h"


int main(int argc, char *argv[]) {

	Controller arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", base);
	arbotix.addMotor(2, "shoulder", shoulder);
	arbotix.addMotor(3, "elbow", elbow);
	arbotix.addMotor(4, "wristAngle", wristAngle);
	arbotix.addMotor(5, "wristRotate", wristRotate);
	arbotix.addMotor(6, "gripper", gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();


	//*
	std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();
	//*/


	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();
	std::cout << arbotix.servosToString();


	/*
	std::cout << "Change ID servomotor 6 to 7." << std::endl;
	arbotix.changeId(6, 7);
	std::cout << arbotix.servosToString();

	std::cout << "Change ID servomotor 7 to 6." << std::endl;
	arbotix.changeId(7, 6);
	std::cout << arbotix.servosToString();
	//*/


	//*
	std::cout << "Change LED of servomotor 2." << std::endl;
	arbotix.turnLED(2);
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 10000);

	std::cout << "Change LED of servomotor 2." << std::endl;
	arbotix.turnLED(2);
	std::cout << arbotix.servosToString();
	//*/


	/*
	std::cout << "Set speed:" << std::endl;
	arbotix.changeSpeed(50);

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
	//*/


}
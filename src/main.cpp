


#include "controller.h"


int main(int argc, char *argv[]) {

	Controller arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ");
	arbotix.addMotor(2, "shoulder");
	arbotix.addMotor(3, "elbow");
	arbotix.addMotor(4, "wristAngle");
	arbotix.addMotor(5, "wristRotate");
	arbotix.addMotor(6, "gripper");
	
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


	/*
	std::cout << "Change LED of servomotor 2." << std::endl;
	arbotix.turnLED(2);
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 10000);

	std::cout << "Change LED of servomotor 2." << std::endl;
	arbotix.turnLED(2);
	std::cout << arbotix.servosToString();
	//*/


	//*
	std::cout << "Set speed:" << std::endl;
	arbotix.changeSpeed(50);

	std::cout << "Set position:" << std::endl;
	arbotix.setPosition({2048, 2048, 2048, 2048, 512, 256});

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();
	std::cout << arbotix.servosToString();
	//*/



}
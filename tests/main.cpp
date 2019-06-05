


#include "serialcontroller.h"
#include "trajectory.h"
#include "cartesianconverter.h"


int main(int argc, char *argv[]) {

	
	SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", base);
	arbotix.addMotor(2, "shoulder", shoulder);
	arbotix.addMotor(3, "elbow", elbow);
	arbotix.addMotor(4, "wristAngle", wristAngle);
	arbotix.addMotor(5, "wristRotate", wristRotate);
	arbotix.addMotor(6, "gripper", gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();



	//*
	// Servo positions to cartesian coordonate system computation
	CartesianConverter conv;
	std::vector<uint16_t> servoPositions = arbotix.getPosition();

	std::cout << "Positions of servomotors : \t";
	for(auto&& v : servoPositions) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<uint16_t> cartesianCoordinates = conv.computeServoToCoord(servoPositions)->get();
	std::cout << "Coordinates equivalent : \t";
	for(auto&& v : cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;


	//*/


}



#include "serialcontroller.h"
#include "trajectory.h"
#include "cartesianconverter.h"


int main(int argc, char *argv[]) {

	/*
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
	arbotix.goToBackhoe();

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();



	std::vector<uint16_t> servoPositions = arbotix.getPosition();
	servoPositions.pop_back(); // Removes gripper
	//*/

	//*
	std::vector<uint16_t> servoPositions = {2048, 2048, 2048, 2048, 512};
	//*/


	//*
	// Servo positions to cartesian coordonate system computation
	CartesianConverter conv;
	conv.addServo("base", rotZ, 0, 0, 1);
	conv.addServo("shoulder", rotX, 0, -2, 5, M_PI/2); // Add 90° because of the orientation of the elbow servomotor
	conv.addServo("elbow", rotX, 0, 0, 5);
	conv.addServo("wristAngle", rotX, 0, 0, 3);
	conv.addServo("wristRotate", rotZ, 0, 0, 2);
	// Useless to add gripper

	std::cout << "Positions of servomotors : ";
	for(auto&& v : servoPositions) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<double> cartesianCoordinates = conv.computeServoToCoord(servoPositions)->getCoord();
	std::cout << "Coordinates equivalent : ";
	for(auto&& v : cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<uint16_t> afterComp_servoPositions = conv.computeCoordToServo(cartesianCoordinates)->getServo();
	std::cout << "Positions got after computation : ";
	for(auto&& v : afterComp_servoPositions) std::cout << v << " ";
	std::cout << std::endl;


	//*/


}
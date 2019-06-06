


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

	//arbotix.setPosition({2048, 1517, 1516, 1025, 512, 256});
	//arbotix.setPosition({2048, 2579, 2586, 3061, 512, 256});
	arbotix.setPosition({2048, 1200, 3000, 2048, 512, 256});

	std::vector<uint16_t> servoPositions = arbotix.getPosition();
	servoPositions.pop_back(); // Removes gripper
	//*/

	//*
	std::vector<uint16_t> servoPositions = {2048, 2048, 2048};
	//*/


	//*
	// Servo positions to cartesian coordonate system computation
	CartesianConverter conv;
	//conv.addServo("stand", fixed, 15, 15, 15); // fixed implies that it does not count as a servomotor (in order to simplify computations)
	conv.addServo("base", fixed, 0, 0, 1, 0, 0, M_PI);
	conv.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	conv.addServo("elbow", rotX, 0, 0, 5);
	conv.addServo("wristAngle", rotX, 0, 0, 3);
	//conv.addServo("wristRotate", rotZ, 0, 0, 2);
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

	std::vector<double> afterComp_cartesianCoordinates = conv.computeServoToCoord(afterComp_servoPositions)->getCoord();
	std::cout << "Coordinates equivalent after computation : ";
	for(auto&& v : afterComp_cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;


	//*/


}
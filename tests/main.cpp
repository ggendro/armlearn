


#include "serialcontroller.h"
#include "trajectory.h"
#include "cylindricalconverter.h"


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
	std::vector<uint16_t> servoPositions = {1500, 2048, 2048, 2048};
	std::vector<uint16_t> reducedServoPositions = std::vector<uint16_t>(servoPositions);
	reducedServoPositions.erase(reducedServoPositions.begin());
	//*/


	//*
	// Servo positions to cylindrical coordinate system computation
	CylindricalConverter convCyl;
	convCyl.addServo("base", rotZ, 0, 0, 1, 0, 0, M_PI);
	convCyl.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	convCyl.addServo("elbow", rotX, 0, 0, 5);
	convCyl.addServo("wristAngle", rotX, 0, 0, 3);


	// Servo positions to cartesian coordinate system computation
	CartesianConverter convCart;
	convCart.addServo("base", fixed, 0, 0, 1, 0, 0, M_PI); // fixed to avoid 3D computations
	convCart.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	convCart.addServo("elbow", rotX, 0, 0, 5);
	convCart.addServo("wristAngle", rotX, 0, 0, 3);

	std::cout << "Positions of servomotors : ";
	for(auto&& v : servoPositions) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<double> cylindricalCoordinates = convCyl.computeServoToCoord(servoPositions)->getCoord();
	std::cout << "Cylindrical coordinates equivalent : ";
	for(auto&& v : cylindricalCoordinates) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<double> cartesianCoordinates = convCart.computeServoToCoord(reducedServoPositions)->getCoord();
	std::cout << "Cartesian coordinates equivalent : ";
	for(auto&& v : cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<uint16_t> afterCompCyl_servoPositions = convCyl.computeCoordToServo(cylindricalCoordinates)->getServo();
	std::cout << "Positions got after cylindrical computation : ";
	for(auto&& v : afterCompCyl_servoPositions) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<uint16_t> afterCompCart_servoPositions = convCart.computeCoordToServo(cartesianCoordinates)->getServo();
	std::cout << "Positions got after cartesian computation : ";
	for(auto&& v : afterCompCart_servoPositions) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<double> afterCompCyl_cylindricalCoordinates = convCyl.computeServoToCoord(afterCompCyl_servoPositions)->getCoord();
	std::cout << "Coordinates equivalent after cylindrical computation : ";
	for(auto&& v : afterCompCyl_cylindricalCoordinates) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<double> afterCompCart_cartesianCoordinates = convCart.computeServoToCoord(afterCompCart_servoPositions)->getCoord();
	std::cout << "Coordinates equivalent after cartesian computation : ";
	for(auto&& v : afterCompCart_cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;


	//*/


}
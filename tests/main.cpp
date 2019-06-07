


#include "serialcontroller.h"
#include "trajectory.h"
#include "optimcartesianconverter.h"
#include "cylindricalconverter.h"


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
	arbotix.goToBackhoe();

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();

	std::vector<uint16_t> servoPositions = arbotix.getPosition();
	uint16_t grip = servoPositions.back();
	servoPositions.pop_back();

	uint16_t rot = servoPositions.back();
	servoPositions.pop_back();


	// Servo positions to cartesian coordinate system computation
	OptimCartesianConverter conv;
	conv.addServo("base", rotZ, 0, 0, 1, 0, 0, M_PI);
	conv.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	conv.addServo("elbow", rotX, 0, 0, 5);
	conv.addServo("wristAngle", rotX, 0, 0, 3);

	bool quit = false;
	double x;
	double y;
	double z;

	while(!quit){
		std::cout << "Continue? y/n" << std::endl;

		std::string rep;
		std::cin >> rep;

		quit = rep == "n";

		if(!quit){

			try{

				std::cout << "Coordinates x y z" << std::endl;
				std::cin >> x >> y >> z;
					
				std::vector<uint16_t> newPositions = conv.computeCoordToServo({x, y, z})->getServo();
				newPositions.push_back(rot);
				newPositions.push_back(grip);

				std::cout << "Corresponding positions : ";
				for(auto&& v : newPositions) std::cout << v << " ";
				std::cout << std::endl;

				arbotix.setPosition(newPositions);

			}catch(OutOfRangeError& e){
				std::cin.clear();
				std::string temp;
				std::cin >> temp;
				std::cout << "Error, incorrect values : " << temp << ". Try again..." << std::endl;
			}catch(ComputationError& e){
				std::cout << "Error during computation : " << e.what() << std::endl;
			}

		}else{
			std::cout << "Quitting..." << std::endl;
		}
	}


}
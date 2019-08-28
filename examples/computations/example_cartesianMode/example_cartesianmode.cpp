
#include <armlearn/serialcontroller.h>
#include <armlearn/trajectory.h>
#include <armlearn/optimcartesianconverter.h>


int main(int argc, char *argv[]) {

    /******************************************/
    /****     Common base for examples     ****/
    /******************************************/

    armlearn::communication::SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", armlearn::communication::base);
	arbotix.addMotor(2, "shoulder", armlearn::communication::shoulder);
	arbotix.addMotor(3, "elbow", armlearn::communication::elbow);
	arbotix.addMotor(4, "wristAngle", armlearn::communication::wristAngle);
	arbotix.addMotor(5, "wristRotate", armlearn::communication::wristRotate);
	arbotix.addMotor(6, "gripper", armlearn::communication::gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


    /******************************************/
    /****     Cartesian mode example       ****/
    /******************************************/

    /*
     * Set arm to backhoe position. Then give to the user a minimalist interface to send cartesian coordinates to the arm.
     * The arm will move accordingly to reach the given coordinates if possible.
     * 
     */

    // Go to backhoe
	arbotix.goToBackhoe();

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();

    // Retrieve positions not used during computation
	std::vector<uint16_t> servoPositions = arbotix.getPosition();
	uint16_t grip = servoPositions.back();
	servoPositions.pop_back();

	uint16_t rot = servoPositions.back();
	servoPositions.pop_back();


	// Servo positions to cartesian coordinate system computation
	armlearn::kinematics::OptimCartesianConverter conv;
	conv.addServo("base", armlearn::kinematics::rotZ, 0, 0, 1, 0, 0, M_PI);
	conv.addServo("shoulder", armlearn::kinematics::rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	conv.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 5);
	conv.addServo("wristAngle", armlearn::kinematics::rotX, 0, 0, 3);

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

			}catch(armlearn::OutOfRangeError& e){
				std::cin.clear();
				std::string temp;
				std::cin >> temp;
				std::cout << "Error, incorrect values : " << temp << ". Try again..." << std::endl;
			}catch(armlearn::ComputationError& e){
				std::cout << "Error during computation : " << e.what() << std::endl;
			}

		}else{
			std::cout << "Quitting..." << std::endl;
		}
	}


}
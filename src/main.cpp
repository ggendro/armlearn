


#include "controller.h"
#include "trajectory.h"


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

	arbotix.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


	/*
	Trajectory path(&arbotix);

	path.addPoint(BACKHOE_POSITION);
	path.addPoint({1024, 2200, 2200, 1025, 512, 511});
	path.addPoint({1024, 2400, 2200, 1200, 512, 511});
	path.addPoint({1024, 2400, 2200, 1200, 512, 135});
	path.addPoint({1024, 2200, 2200, 1025, 512, 135});
	path.addPoint({2048, 2200, 2200, 1025, 512, 135});
	path.addPoint({2048, 2400, 2200, 1200, 512, 135});
	path.addPoint({2048, 2400, 2200, 1200, 512, 511});
	path.addPoint({2048, 2200, 2200, 1025, 512, 511});
	path.addPoint(SLEEP_POSITION);

	path.printTrajectory();


	path.init();
	path.executeTrajectory();

	//*/


}
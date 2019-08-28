

#include "serialcontroller.h"
#include "trajectory.h"

using namespace armlearn;

int main(int argc, char *argv[]) {

    /******************************************/
    /****     Common base for examples     ****/
    /******************************************/

    communication::SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", communication::base);
	arbotix.addMotor(2, "shoulder", communication::shoulder);
	arbotix.addMotor(3, "elbow", communication::elbow);
	arbotix.addMotor(4, "wristAngle", communication::wristAngle);
	arbotix.addMotor(5, "wristRotate", communication::wristRotate);
	arbotix.addMotor(6, "gripper", communication::gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();


	std::this_thread::sleep_for((std::chrono::milliseconds) 1000); // Usually, a waiting period and a second connect attempt is necessary to reach all devices
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50); // Servomotor speed is reduced for safety

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


    /******************************************/
    /****    Up and down loop example      ****/
    /******************************************/

    /*
     * Set arm to backhoe position. Grab and move an invisible object located at its right in front of him and drop it. Go to sleep position.
     * 
     */
    arbotix.changeSpeed(100);

    Trajectory init(&arbotix);
	init.addPoint({2500, 1800, 1800, 2100, 815, 500});
	init.addPoint({2500, 1800, 1800, 2100, 815, 500});
	init.addPoint({2500, 1800, 1800, 2100, 815, 5});

	init.printTrajectory();

    init.init();
    init.executeTrajectory();


    Trajectory loop(&arbotix);

	loop.addPoint({2500, 1800, 1600, 2100, 815, 5});
	loop.addPoint({2500, 1800, 2000, 2100, 815, 5});

	loop.printTrajectory();

    while(true){
        loop.init();
        loop.executeTrajectory();

    }



}
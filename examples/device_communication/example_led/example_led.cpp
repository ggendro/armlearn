

#include <armlearn/serialcontroller.h>
#include <armlearn/widowxbuilder.h>


int main(int argc, char *argv[]) {

    /******************************************/
    /****     Common base for examples     ****/
    /******************************************/

    armlearn::communication::SerialController arbotix("/dev/ttyUSB0");

	armlearn::WidowXBuilder builder;
	builder.buildController(arbotix);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();


	std::this_thread::sleep_for((std::chrono::milliseconds) 1000); // Usually, a waiting period and a second connect attempt is necessary to reach all devices
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50); // Servomotor speed is reduced for safety

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();


    /******************************************/
    /****            LED example           ****/
    /******************************************/

    /*
     * Turn on LED of servomotor with ID 2, usually the shoulder servomotor. Wait 10 seconds and turn it off. 
     * 
     */

    int id = 2;

    std::cout << "Turn ON LED of servomotor "  << id << "." << std::endl;
	arbotix.turnLED(id);
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 10000);

	std::cout << "Turn OFF LED of servomotor  " << id << "." << std::endl;
	arbotix.turnLED(id);
	std::cout << arbotix.servosToString();

}
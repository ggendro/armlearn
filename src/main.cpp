


#include "controller.h"


int main(int argc, char *argv[]) {


	//Connection to servomotors
	//*
	Controller arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ");
	arbotix.addMotor(2, "shoulder");
	arbotix.addMotor(3, "elbow");
	arbotix.addMotor(4, "wristAngle");
	arbotix.addMotor(5, "wristRotate");
	arbotix.addMotor(6, "gripper");
	
	arbotix.connect();
	std::cout << arbotix.servosToString();

	std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.updateInfos();
	std::cout << arbotix.servosToString();

	//*/


	// Other baud connection
	/*
	serial::Serial newSerialPort("/dev/ttyUSB0", 115200);
    if(!newSerialPort.isOpen()) throw ConnectionError("Error while connecting"); 
	newSerialPort.flush();

    //std::vector<uint8_t> sendBuf = {0x06, 0x05, 0x03, 0x1E, 0x2, 0x00};
	//std::vector<uint8_t> sendBuf = {0x06, 0x05, 0x03, 0x1E, 0xFF, 0x00};
	std::vector<uint8_t> sendBuf = {0x06, 0x04, 0x02, 0x03, 0x01};

    int sum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = sendBuf.cbegin(); ptr < sendBuf.cend(); ptr++){
		sum += *ptr;
	}
	sendBuf.push_back(255 - (sum % 256));


    sendBuf.insert(sendBuf.begin(), 0xFF);
    sendBuf.insert(sendBuf.begin(), 0xFF);

    

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

    while(newSerialPort.available() < 5 && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < 20000){
		newSerialPort.flushInput();
		int res = newSerialPort.write(sendBuf);

		std::cout << "Packet sent at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << " : " << std::endl;
		for (auto&& x : sendBuf) std::cout << (int) x << " ";
		std::cout << "(" << res << ")" << std::endl;

        std::this_thread::sleep_for((std::chrono::milliseconds) 5000);
		currentTime = std::chrono::system_clock::now();
    }

	std::vector<uint8_t> buffer;

    int nbChar = newSerialPort.available();
    int res2 = newSerialPort.read(buffer, nbChar);

    std::cout << "Message received : ";
    for(auto&& v : buffer) {
        std::cout << (int) v << " ";
    }
    std::cout << "(" << res2 << ")" << std::endl;
	//*/

}
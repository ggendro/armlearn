
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <iterator>
#include <vector>
#include <serial/serial.h>

#include "widowx.h"
#include "trajectory.h"




// To remove when dynamixel_sdk tests are over
/*
#include <dynamixel_sdk/dynamixel_sdk.h>
#include <fcntl.h>
#include <termios.h>

// Control table address
#define ADDR_PRO_TORQUE_ENABLE          562                 // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          596
#define ADDR_PRO_PRESENT_POSITION       611

// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define DXL_ID                          1                   // Dynamixel ID: 1
#define BAUDRATE                        1000000
#define DEVICENAME                      "/dev/ttyUSB0"      // Check which port is being used on your controller
                                                            // ex) Windows: "COM1"   Linux: "/dev/ttyUSB0"

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MINIMUM_POSITION_VALUE      -150000             // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      150000              // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold

#define ESC_ASCII_VALUE                 0x1b
//*/





int main(int argc, char *argv[]) {


	// Communication API code tests
	/*

	WidowX rob("/dev/ttyUSB0");

	int stay = 1;
	while(stay){
		std::cout << "what's next? \n0. quit \n1. move \n2. change mode" <<std::endl;
		std::cin >> stay;

		if(stay == 2){
			std::cout << "mode: \n0. Straight cartesian \n1. 90 degrees cartesian \n2. Straight cylindrical \n3. 90 degrees cylindrical \n4. Backhoe \n5. Sleeping" <<std::endl;

			int newMode;
			std::cin >> newMode;
			rob.changeMode((Mode) newMode);

		}else if (stay == 1){
			std::cout << "values:" <<std::endl;

			std::vector<int> values;
			for(int i=0; i<6; i++){
				int newVal;
				std::cin >> newVal;

				values.push_back(newVal);
			}

			rob.move(values);
		}

	}
	//*/


	//Communication API tests of dynamixel_sdk library
	/*
	
	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
  	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

	if (!portHandler->openPort()){
		std::cout << "Error while opening the port - exit" << std::endl;
		return 0;
	}

	if(!portHandler->setBaudRate(BAUDRATE)){
		std::cout << "Error while setting baudrate - exit" << std::endl;
		return 0;
	}

	portHandler->setPacketTimeout(100000.0);
	int index = 0;
	int dxl_comm_result = COMM_TX_FAIL;             // Communication result
	int dxl_goal_position[2] = {DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE};         // Goal position

	uint8_t dxl_error = 0;                          // Dynamixel error
	int32_t dxl_present_position = 0;               // Present position

	// Enable Dynamixel Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		const char* res = packetHandler->getTxRxResult(dxl_comm_result);
		printf("%s\n", res);
	}
	else if (dxl_error != 0)
	{
		const char* res = packetHandler->getRxPacketError(dxl_error);
		printf("%s\n", res);
	}
	else
	{
		printf("Dynamixel has been successfully connected \n");
	}

	//*/


	//Communication API send and receive message tests (in widowx.h, put send and receive methods in public)
	/*
	WidowX rob("/dev/ttyUSB0");
	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
	int timeout = 10000;

	while(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < timeout){
		std::vector<uint8_t> sendBuf{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x81};
		rob.send(sendBuf);
	
		std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
		std::vector<uint8_t> receiveBuf;
		rob.receive(receiveBuf);

		currentTime = std::chrono::system_clock::now();
	}

	//*/
	

	//Serial API communication with servomotors
	/*
	*			byte id: id of the servo to send data to
 	*           byte instruction: kind of instruction to send to servo
 	*                              1-Ping
 	*                              2-READ_DATA
	*                              3-WRITE_DATA
	*                              4-REG_WRITE
	*                              5-ACTION
	*                              6-RESET
	*                              83-SYNC WRITE
	*                              more info at  http://support.robotis.com/en/product/dynamixel/communication/dxl_packet.htm
	*           byte parameters: parameters for the packet.
	*                            Writes : [0] ->starting register
	*                                     [1] -> first value (at register [0])
	*                                     [2] -> first value (at register [0]+1)(optional)
	*                                     etc(optional)
	*                             Reads : [0] ->starting register
	*                                     [1] -> length (how many registers to read)
	*/
	/*

	serial::Serial serialPort("/dev/ttyUSB0", 34800); // For arbotix : 34800 (that's the good one) ; for servomotor MX-64 : 57600 or 1000000 according to dynamixel_SDK and dynaManager
	
	
	

	uint8_t id = 1; // Put the ID of the servo to send the packet to, OxFE for broadcast
	uint8_t instruction = 2;
	std::vector<uint8_t> parameters = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x81};
	uint8_t length = 2 + parameters.size();

	uint8_t checksum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = parameters.cbegin(); ptr < parameters.cend(); ptr++){
		checksum += *ptr;
	}

	checksum = 255 - ((id + length + instruction + checksum) % 256);

	std::vector<uint8_t> sendBuf = {HEADER, HEADER, id, length, instruction};
    sendBuf.insert(sendBuf.end(), parameters.begin(), parameters.end());
	sendBuf.push_back(checksum);

    int sent = serialPort.write(sendBuf);

	std::cout << "Message sent : ";
    for(auto&& v : sendBuf) std::cout << (int) v << " ";
    std::cout << "(" << sent << ")" << std::endl;

	

	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

	while(serialPort.available() < MIN_PACKET_SIZE && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < 10000){
		currentTime = std::chrono::system_clock::now();
		std::cout << "No answer at " <<  std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;

		std::this_thread::sleep_for((std::chrono::milliseconds) 100); // Will check every x milliseconds
	}
	std::cout << "Answer at " <<  std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
	std::this_thread::sleep_for((std::chrono::milliseconds) 250); // Leave time for the buffer to complete filling

	std::vector<uint8_t> receiveBuf;

    int nbChar = serialPort.available();
    int receive = serialPort.read(receiveBuf, nbChar);

	std::cout << "Message received : ";
    for(auto&& v : receiveBuf) std::cout << v << " ";
    std::cout << "(" << receive << ")" << std::endl;
	

	//*/


	//Trajectory API receive message tests
	/*
	WidowX* rob = new WidowX("/dev/ttyUSB0");

	Trajectory path(rob);

	path.addPoint({2048, 2048, 2048, 2048, 512, 159});

	path.printTrajectory();

	int i = fork();
	if(i){

		path.init();
		path.executeTrajectory();

		std::this_thread::sleep_for((std::chrono::milliseconds) 15000);

		delete rob;

	}else{

		int nbRead = 50;
		int realRead = 0;
        std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
		int timeout = 10000;

		while(realRead < nbRead && std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < timeout){
			std::this_thread::sleep_for((std::chrono::milliseconds) 1000);
			std::vector<uint8_t> receiveBuf;
			rob->receive(receiveBuf);

			if(receiveBuf.size() > 0){
				for(auto v : receiveBuf) std::cout << (int) v << "\t";
				std::cout << std::endl;
			}
			currentTime = std::chrono::system_clock::now();
		}

		delete rob;

	}

	//*/


	//Trajectory API code tests
	//*
	WidowX* rob = new WidowX("/dev/ttyUSB0");

	Trajectory path(rob);

	path.addPoint({2048, 2048, 2048, 2048, 512, 159});
	path.addPoint({2048, 1900, 2800, 1400, 512, 256});
	path.addPoint({2048, 2400, 1900, 2400, 256, 10});
	path.addPoint({2825, 2048, 2048, 2048, 768, 159});

	path.printTrajectory();


	path.init();
	path.executeTrajectory();

	delete rob;

	//*/

}
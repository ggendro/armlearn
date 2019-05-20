
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <iterator>
#include <vector>
#include <serial/serial.h>



#define BYTES_READ 10

int main(int argc, char *argv[]) {

	/*

	serial::Serial port("/dev/ttyUSB0");

	if(!port.isOpen()) port.open();

	std::string readingBuf;
	std::string writingBuf;

	size_t resRead = port.read(readingBuf, BYTES_READ);

	std::cout << "Buffer read (" << resRead << ") : " << readingBuf << std::endl;

	int serialHeader = 255; // Header

	std::vector<int> data(7);
	data[0] = 0; // X
	data[1] = 0; // Y
	data[2] = 0; // Z
	data[3] = 0; // Wrist angle
	data[4] = 0; // Wrist rotate
	data[5] = 0; // Gripper
	data[6] = 0; // Delta

	int enableOutput = 0; // Enable output
	int moveCode = 96; // 0 to move according to previous values, 64 to go to backhoe mode, 96 to go to sleep mode

	int checksum = 0;
	
	writingBuf += serialHeader;
	std::cout << writingBuf << std::endl;

	for(std::vector<int>::iterator ptr = data.begin(); ptr < data.end(); ptr++){
		checksum += *ptr;
		writingBuf += *ptr;
	}
	checksum = 255 - ((checksum + enableOutput + moveCode) % 256);
	writingBuf += checksum;

	std::cout << "Sending message :  " << writingBuf << std::endl;
	
	size_t resWrite = port.write(writingBuf);

	std::cout << "Res :  " << resWrite << std::endl;


	resRead = port.read(readingBuf, BYTES_READ);

	std::cout << "Buffer read (" << resRead << ") : " << readingBuf << std::endl;
	


	port.close();
	std::cout << "Executed" << std::endl;

	*/

	int value = 256;
	int size = sizeof(int);

	char* arr = static_cast<char*>(static_cast<void*>(&value));

	uint8_t val1 = (int) arr[0] << 1;
	

	std::cout << val1 << " + " /*<< val2 */<< std::endl; 

}
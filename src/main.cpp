
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <serial.h>

#define BYTES_READ 10

int main(int argc, char *argv[]) {

	serial::Serial port("/dev/ttyUSB0");

	if(!port.isOpen()) port.open();

	std::string buf;
	size_t res = port.read(buf, BYTES_READ);

	std::cout << buf << std::endl;



	port.close();

}

#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <iterator>
#include <vector>
#include <serial/serial.h>

#include "widowx.h"


int main(int argc, char *argv[]) {

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

}
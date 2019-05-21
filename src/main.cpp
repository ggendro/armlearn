
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

	bool stay = true;
	while(stay){
		std::cout << "what's next?" <<std::endl;
		std::cin >> stay;

		if(stay == 32){
			std::cout << "Cartesian mode" <<std::endl;
			//rob.goToCartesian();

		}else if(stay == 64){
			std::cout << "Backhoe mode" <<std::endl;
			//rob.goToBackhoe();

		}else if(stay == 96){
			std::cout << "Sleep mode" <<std::endl;
			//rob.goToSleep();

		}else if (stay > 0){
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
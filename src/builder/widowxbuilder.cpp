/**
 * @copyright Copyright (c) 2019
 */

#include "widowxbuilder.h"



WidowXBuilder::WidowXBuilder(){

}

WidowXBuilder::~WidowXBuilder(){
    
}


void WidowXBuilder::buildController(AbstractController& controller){
    controller.addMotor(1, "base ", base);
	controller.addMotor(2, "shoulder", shoulder);
	controller.addMotor(3, "elbow", elbow);
	controller.addMotor(4, "wristAngle", wristAngle);
	controller.addMotor(5, "wristRotate", wristRotate);
	controller.addMotor(6, "gripper", gripper);
}

void WidowXBuilder::buildConverter(Converter& converter){ // TODO: look for real vector values
    converter.addServo("base", rotZ, 0, 0, 1, 0, 0, M_PI);
	converter.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	converter.addServo("elbow", rotX, 0, 0, 5);
	converter.addServo("wristAngle", rotX, 0, 0, 3);
}


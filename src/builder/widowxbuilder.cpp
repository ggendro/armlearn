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

void WidowXBuilder::buildConverter(Converter& converter){
    converter.addServo("base", rotZ, 0, 0, 125, 0, 0, M_PI);
	converter.addServo("shoulder", rotX, 0, -49, 142, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	converter.addServo("elbow", rotX, 0, 0, 142);
	converter.addServo("wristAngle", rotX, 0, 0, 74);
	converter.addServo("wristRotate", rotZ, 0, 0, 41);
	converter.addServo("gripper", rotZ, 0, 0, 40);
}


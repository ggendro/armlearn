/**
 * @copyright Copyright (c) 2019
 */

#include "widowxbuilder.h"

using namespace armlearn;

WidowXBuilder::WidowXBuilder(){

}

WidowXBuilder::~WidowXBuilder(){
    
}


void WidowXBuilder::buildController(communication::AbstractController& controller){
    controller.addMotor(1, "base ", communication::base);
	controller.addMotor(2, "shoulder", communication::shoulder);
	controller.addMotor(3, "elbow", communication::elbow);
	controller.addMotor(4, "wristAngle", communication::wristAngle);
	controller.addMotor(5, "wristRotate", communication::wristRotate);
	controller.addMotor(6, "gripper", communication::gripper);
}

void WidowXBuilder::buildConverter(kinematics::Converter& converter){
    converter.addServo("base", kinematics::rotZ, 0, 0, 125, 0, 0, M_PI);
	converter.addServo("shoulder", kinematics::rotX, 0, -49, 142, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	converter.addServo("elbow", kinematics::rotX, 0, 0, 142);
	converter.addServo("wristAngle", kinematics::rotX, 0, 0, 74);
	converter.addServo("wristRotate", kinematics::rotZ, 0, 0, 41);
	converter.addServo("gripper", kinematics::rotZ, 0, 0, 40);
}


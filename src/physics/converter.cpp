/**
 * @copyright Copyright (c) 2019
 */


#include "converter.h"


Converter::Converter():lastCoord(), lastServo(){
    device = new KDL::Chain();
}

Converter::~Converter(){
    delete device;
}


Converter* Converter::addServo(const std::string& name, Axis axis, double lengthX, double lengthY, double lengthZ, double rot){

    KDL::Joint::JointType joint;
    KDL::Rotation rotFrame;
    
    switch (axis){
        case rotX:
            joint = KDL::Joint::RotX;
            rotFrame.DoRotX(rot);
            break;

        case rotY:
            joint = KDL::Joint::RotY;
            rotFrame.DoRotY(rot);
            break;

        case rotZ:
            joint = KDL::Joint::RotZ;
            rotFrame.DoRotZ(rot);
            break;

        case transX:
            joint = KDL::Joint::TransX;
            break;

        case transY:
            joint = KDL::Joint::TransY;
            break;

        case transZ:
            joint = KDL::Joint::TransZ;
            break;
        
        default:
            break;
    }

    device->addSegment(KDL::Segment(name, KDL::Joint(joint), KDL::Frame(rotFrame, KDL::Vector(lengthX, lengthY, lengthZ))));
    return this;
}

Converter* Converter::removeAllServos(){
    delete device;
    device = new KDL::Chain();

    return this;
}


std::vector<double> Converter::getCoord() const{
    return lastCoord;
}

std::vector<uint16_t> Converter::getServo() const{
    return lastServo;
}
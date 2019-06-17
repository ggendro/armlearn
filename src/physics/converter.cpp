/**
 * @copyright Copyright (c) 2019
 */


#include "converter.h"


Converter::Converter():lastCoord(), lastServo(), nbServos(0){
    device = new KDL::Chain();
}

Converter::~Converter(){
    delete device;
}


Converter* Converter::addServo(const std::string& name, Axis axis, double lengthX, double lengthY, double lengthZ, double rotationX, double rotationY, double rotationZ){
    nbServos++;

    KDL::Joint::JointType joint;
    switch (axis){
        case rotX:
            joint = KDL::Joint::RotX;
            break;

        case rotY:
            joint = KDL::Joint::RotY;
            break;

        case rotZ:
            joint = KDL::Joint::RotZ;
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
        
        case fixed:

        default:
            joint = KDL::Joint::None;
            nbServos--; // If servomotor not moveable, is not counted as servomotor during computation
            break;
    }

    KDL::Rotation rotFrame;
    rotFrame.DoRotX(rotationX);
    rotFrame.DoRotY(rotationY);
    rotFrame.DoRotZ(rotationZ);

    device->addSegment(KDL::Segment(name, KDL::Joint(joint), KDL::Frame(rotFrame, KDL::Vector(lengthX, lengthY, lengthZ))));
    return this;
}

Converter* Converter::removeAllServos(){
    delete device;
    device = new KDL::Chain();
    nbServos = 0;

    return this;
}


std::vector<double> Converter::getCoord() const{
    return lastCoord;
}

std::vector<uint16_t> Converter::getServo() const{
    return lastServo;
}

int Converter::getNbServos() const{
    return nbServos;
}
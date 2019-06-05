/**
 * @copyright Copyright (c) 2019
 */


#include "converter.h"


Converter::Converter():lastInput(), lastOutput(){
    device = new KDL::Chain();

    device->addSegment(KDL::Segment());

}

Converter::~Converter(){
    delete device;
}


std::vector<uint16_t> Converter::get(){
    return lastOutput;
}

std::vector<uint16_t> Converter::getInput(){
    return lastInput;
}
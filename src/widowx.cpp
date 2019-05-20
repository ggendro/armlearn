
#include "widowx.h"

WidowX::WidowX(const std::string port){
    serialPort = new serial::Serial(port);
    this->goToBackhoe();
}

WidowX::~WidowX(){
    serialPort->close();
    this->goToSleep();

    delete serialPort;
}


void WidowX::setPositions(uint16_t* values){
    for(int i=0; i< NB_BIG_VALUE_MOTORS; i++){
        this->positions[i] = values[i];
    }
}

void WidowX::setDelta(uint8_t value) {
    this->delta = value;
}


void WidowX::open(){
    if(!serialPort->isOpen()) serialPort->open();    
}

uint8_t WidowX::computeChecksum(const std::vector<uint8_t>& data){
    int sum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = data.cbegin(); ptr < data.cend(); ptr++){
		sum += *ptr;
	}

	return 255 - (sum % 256);
}


int WidowX::send(const std::vector<uint8_t>& buffer){
    this->open();

    std::vector<uint8_t> fullBuf(buffer);

    fullBuf.push_back(HEADER);
    fullBuf.push_back(this->computeChecksum(buffer));

    this->serialPort->write(fullBuf);

}


int WidowX::receive(std::vector<uint8_t>& buffer){
    this->open();
    int nbChar = serialPort->available();
    
    this->serialPort->read(buffer, nbChar);
}



void WidowX::move(const std::vector<int>& positions){
    //Set the new position
    uint16_t formattedPos[NB_BIG_VALUE_MOTORS];

    for(int i=0; i < NB_BIG_VALUE_MOTORS; i++){
        formattedPos[i] = (uint16_t) positions[i];
    }

    this->setPositions(formattedPos);

    //Send the new position to the board
    std::vector<uint8_t> posCodes;
    for(std::vector<int>::const_iterator ptr = positions.begin(); ptr < positions.end(); ptr++){
        posCodes.push_back(static_cast<uint8_t>(*ptr));
        posCodes.push_back(static_cast<uint8_t>((*ptr >> 8)));
    }

    posCodes.push_back(this->delta);
    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(MOVE_MODE);

    this->send(posCodes);
}

void WidowX::changeSpeed(int newSpeed){
    this->setDelta(newSpeed);
}

void WidowX::goToBackhoe(){
    //Set the new position
    uint16_t arr[] = BACKHOE_POSITION;
    this->setPositions(arr);

    //Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(BACKHOE_MODE);

    this->send(posCodes);
}

void WidowX::goToSleep(){
    //Set the new position
    uint16_t arr[] = SLEEP_POSITION;
    this->setPositions(arr);

    //Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(SLEEP_MODE);

    this->send(posCodes);
}

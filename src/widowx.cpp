
#include "widowx.h"

WidowX::WidowX(const std::string port):id(0), delta(DEFAULT_SPEED){
    uint16_t arr[] = SLEEP_POSITION;
    this->setPositions(arr);

    serialPort = new serial::Serial(port, 38400); // Default values are defined in the Arm Link Reference
    this->open();

    if(!this->connect()) {
        throw std::runtime_error("Unable to connect to the device - timeout"); // TODO: create a specific exception
    }
        
    
    this->goToBackhoe();
}

WidowX::~WidowX(){
    this->goToSleep();
    serialPort->close();

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

bool WidowX::connect(){
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    std::chrono::milliseconds wait(100);

    std::vector<uint8_t> messageSent{0x00, 0x00, 0x00, 0xC8, 0x00, 0xC8, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x80, 0x00, CONNECT_REQUEST}; // first bytes are unconsequential, only the last byte is important
    
    while(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < CONNECT_TIMEOUT){
        this->send(messageSent);

        std::vector<uint8_t> messageReceived;
        int nbRead = this->receive(messageReceived);

        if(nbRead > 0 && this->checkValidity(messageReceived)){
            //todo: complete verification, collect id
            return true;
        }

        std::this_thread::sleep_for(wait);
        currentTime = std::chrono::system_clock::now();
        std::cout << "Failed to connect after " <<  std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
    }
    
    return false;
}

uint8_t WidowX::computeChecksum(const std::vector<uint8_t>& data){
    int sum = 0;
    for(std::vector<uint8_t>::const_iterator ptr = data.cbegin(); ptr < data.cend(); ptr++){
		sum += *ptr;
	}

	return 255 - (sum % 256);
}

/**
 * @brief Test that the packet has a valid format
 * 
 * @param data the packet to verify
 * @return true if the packet is valid
 * @return false if not
 * 
 * TODO: be able to do specific packets check, such as id verification
 */
bool WidowX::checkValidity(const std::vector<uint8_t>& data){

    //Verify that the first byte matches with the header
    if((int) data.front() != HEADER) {
        std::cout << "Wrong header : " << (int) data.front() << " - required : " << HEADER << std::endl;
        return false;
    }

    //Verify that the checksum is correct
    int val = 0;
    for(std::vector<uint8_t>::const_iterator ptr = data.cbegin()+1; ptr < data.cend()-1; ptr++){
        val += *ptr;
    }
    if (255 - (val % 256) != data.back()) return false;

    return true;
}

/**
 * @brief Send a packet to the connected serial port
 * 
 * @param buffer the content of the packet
 * @return int the number of bytes really sent
 * 
 * buffer size has to be 15 to be valid
 */
int WidowX::send(const std::vector<uint8_t>& buffer){
    //this->open();

    std::vector<uint8_t> fullBuf(buffer);

    fullBuf.insert(fullBuf.begin(), HEADER);
    fullBuf.push_back(this->computeChecksum(buffer));

    int res = this->serialPort->write(fullBuf);
    //*
    std::cout << "Packet sent : ";
    for (auto&& x : fullBuf) std::cout << std::hex << (int) x << " ";
    std::cout << "(" << res << ")" << std::endl;
    //*/

    return res;
}


int WidowX::receive(std::vector<uint8_t>& buffer){
    //this->open();
    int nbChar = serialPort->available();
    
    int res = this->serialPort->read(buffer, nbChar);

    //*
    std::cout << "Message received : ";
    for(auto v : buffer) std::cout << v << " ";
    std::cout << "(" << res << ")" << std::endl;
    for(auto v : buffer) std::cout << (int) v << " ";
    std::cout << "(converted into integers)" << std::endl;
    //*/
    return res;
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
        posCodes.push_back(static_cast<uint8_t>((*ptr >> 8)));
        posCodes.push_back(static_cast<uint8_t>(*ptr));
    }

    posCodes.push_back(this->delta);
    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(MOVE_MODE);

    this->send(posCodes);

    std::vector<uint8_t> response;
    this->receive(response);
}

void WidowX::changeSpeed(int newSpeed){
    this->setDelta(newSpeed);
}

void WidowX::goToBackhoe(){
    //Set the new position
    uint16_t arr[] = BACKHOE_POSITION;
    this->setPositions(arr);
    this->changeSpeed(DEFAULT_SPEED);

    //Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(BACKHOE_MODE);

    this->send(posCodes);

    std::vector<uint8_t> response;
    this->receive(response);
}

void WidowX::goToCartesian(){
    //Set the new position
    uint16_t arr[] = CARTESIAN_POSITION;
    this->setPositions(arr);
    this->changeSpeed(DEFAULT_SPEED);

    //Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(CARTESIAN_MODE);

    this->send(posCodes);

    std::vector<uint8_t> response;
    this->receive(response);
}

void WidowX::goToSleep(){
    //Set the new position
    uint16_t arr[] = SLEEP_POSITION;
    this->setPositions(arr);
    this->changeSpeed(DEFAULT_SPEED);

    //Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(SLEEP_MODE);

    this->send(posCodes);

    std::vector<uint8_t> response;
    this->receive(response);
}

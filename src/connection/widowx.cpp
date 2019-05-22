/**
 * @copyright Copyright (c) 2019
 */


#include "widowx.h"

/**
 * @brief Construct a new Widow X:: Widow X object
 * 
 * @param port string format, path to the used port
 * 
 * Creates a new WidowX object and connects to the physical WidowXdevice. Sets it to backhoe mode.
 */
WidowX::WidowX(const std::string port):mode(sleeping), delta(DEFAULT_SPEED){

    ranges = new std::vector<Range*>();

    serialPort = new serial::Serial(port, 38400); // Default values are defined in the Arm Link Reference
    this->open();

    if(!this->connect()) {
        throw ConnectionError("Unable to connect to the device - timeout");
    }
        
    
    this->changeMode(backhoe);
}

/**
 * @brief Destroy the Widow X:: Widow X object
 * 
 */
WidowX::~WidowX(){
    this->changeMode(sleeping);
    serialPort->close();

    delete serialPort;

    for(auto ptr = ranges->begin(); ptr < ranges->end(); ptr++){
        delete *ptr;
    }
    delete ranges;
}


/**
 * @brief Set the position of the motors
 * 
 * @param values the new positions of the motors
 * 
 * Method non accessible for user, called by move() or a goTo() mehod
 */
void WidowX::setPositions(const std::vector<uint16_t>& values){
    for(int i=0; i< NB_BIG_VALUE_MOTORS; i++){
        this->positions[i] = values[i];
    }
}

/**
 * @brief Set the delta of the motors
 * 
 * @param value the new delta value
 * 
 * Method non accessible for user, called by move() or a goTo() mehod
 */
void WidowX::setDelta(const uint8_t value) { // TODO: add range test
    this->delta = value;
}


/**
 * @brief Open the serial port if not already done
 * 
 */
void WidowX::open(){
    if(!serialPort->isOpen()) serialPort->open();    
}

/**
 * @brief Attempt to connect to the device
 * 
 * @return true if connection succeeded
 * @return false if connection failed
 * 
 * The method will send successively connection requests to the device for CONNECT_TIMEOUT ms, when receiving  response, will call a method to check the validity of the device
 */
bool WidowX::connect(){
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    std::chrono::milliseconds wait(100);

    std::vector<uint8_t> messageSent{0x00, 0x00, 0x00, 0xC8, 0x00, 0xC8, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x80, 0x00, CONNECT_REQUEST}; // first bytes are unconsequential, only the last byte is important
    
    while(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() < CONNECT_TIMEOUT){
        this->send(messageSent);

        std::vector<uint8_t> messageReceived;
        int nbRead = this->receive(messageReceived);

        if(nbRead > 0 && this->checkValidity(messageReceived)){ // Generic packet verification

            if(messageReceived.size() == 5 && messageReceived[3] == 0 && messageReceived[1] == VALID_ID){ // Verification for id type packet: error code equals 0 and id code equals VAILD_ID
                std::cout << "Connection established at " << std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
                return true;
            }
        }

        std::this_thread::sleep_for(wait);
        currentTime = std::chrono::system_clock::now();
        std::cout << "Failed to connect after " <<  std::chrono::duration<double, std::ratio<1, 1>>(currentTime - startTime).count() << " s" << std::endl;
    }
    
    return false;
}

/**
 * @brief Computes the checksum of a packet
 * 
 * @param data the data containedin the packet
 * @return uint8_t the according checksum
 */
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
 * Verifies that the header and the checksum are correct
 */
bool WidowX::checkValidity(const std::vector<uint8_t>& data){

    //Verify that the first byte matches with the header
    if((int) data.front() != HEADER) {
        std::cout << "Wrong header : " << (int) data.front() << " - required : " << HEADER << std::endl;
        return false;
    }

    //Verify that the checksum is correct
    std::vector<uint8_t> reducedData((data.cbegin()+1), (data.cend()-1)); // removes the header and the actual checksum from data
    if (this->computeChecksum(reducedData) != data.back()) return false;

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

/**
 * @brief Fills a buffer with the input from the device
 * 
 * @param buffer the buffer to fill data with
 * @return int the number of bytes read
 */
int WidowX::receive(std::vector<uint8_t>& buffer){
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

/**
 * @brief Move directly the device without prior checks
 * 
 * @param positions 
 */
void WidowX::forceMove(const std::vector<uint16_t>& positions){
    //Set the new position
    this->setPositions(positions);

    //Send the new position to the board
    std::vector<uint8_t> posCodes;
    for(std::vector<uint16_t>::const_iterator ptr = positions.begin(); ptr < positions.end(); ptr++){
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



/**
 * @brief Moves the device to the positions set in parameter
 * 
 * @param positions the positions to move the servomotors to
 * 
 * TODO : add limits to the positions to make impossible to go to undesired positions such as ones that overlaps on other board elements
 */
void WidowX::move(const std::vector<int>& positions){
    // Verify if moving the robot is currently possible
    if (!this->isMoveEnabled()) throw MovementError("Device movement not enabled : sleeping mode - change mode to call method");

    // Verify that input is valid
    if (!this->isMoveValid(positions)) throw MovementError("Movement not allowed : value out of range");//TODO: better throw a OutOfRangeError exception

    // Format the input
    std::vector<uint16_t> formattedPos(positions.cbegin(), positions.cend());

    /*for(int i=0; i < NB_BIG_VALUE_MOTORS; i++){
        formattedPos[i] = (uint16_t) positions[i];
    }*/

    // Execute the move
    this->forceMove(formattedPos);
}

void WidowX::changeSpeed(int newSpeed){
    this->setDelta(newSpeed);
}

/**
 * @brief Check if it is possible to execute a move
 * 
 * @return true 
 * @return false 
 */
bool WidowX::isMoveEnabled() const{
    return this->mode != sleeping;
}

/**
 * @brief Check if the move in parameter is possible
 * 
 * @param positions the positions to move the device to
 * @return true if all positions are valid
 * @return false if at least one position is not correct
 * 
 * Only backhoe mode is implemented, will return false if current mode is not backhoe
 * TODO: implement verification for other modes as well
 */
bool WidowX::isMoveValid(const std::vector<int>& positions) const{

    //*
    if(this->mode != backhoe) {
        std::cerr << std::endl  << "/!\\ ---------------------------------------------------------/!\\ " << std::endl 
                                << "/!\\ Safety not implemented : Movement disabled for this mode /!\\ " << std::endl
                                << "/!\\ ---------------------------------------------------------/!\\ " << std::endl;
    
        return false;
    }    
    //*/

    // TODO: complete

    return true;
}


/**
 * @brief Change the current working mode
 * 
 * @param newMode the new mode of the device
 * 
 * Possible modes:
 * 
 *  - cartesianStraight : moves the arm according to a cartesian coordonate system, straight positionning at initialization
 *       }--\
 *          |
 *       __/
 *      |__|
 * 
 *  - cartesian90degrees : moves the arm according to a cartesian coordonate system, 90 degrees positionning at initialization
 *    /----\
 *   |-| __/
 *      |__|
 * 
 *  - cylindricStraight : moves the arm according to a cylindric coordonate system, straight positionning at initialization
 *       }--\
 *          |
 *       __/
 *      |__|
 * 
 *  - cylindric90degrees : moves the arm according to a cylindric coordonate system, 90 degrees positionning at initialization
 *    /----\
 *   |-| __/
 *      |__|
 * 
 *  - backhoe : commands directly the arm's servomotors, right angle at positionning
 *    }-----
 *         |
 *       __|
 *      |__|
 * 
 *  - sleeping : disables the arm's servomotors, tidy position
 *       }----\
 *       __/--/
 *      |__|
 */
void WidowX::changeMode(Mode newMode){
    //*
    std::cout << "Changing from " << modeToString(this->mode) << " to " << modeToString(newMode) << std::endl;
    this->mode = newMode;
    //*/

    std::vector<uint16_t> pos;
    int code;
    bool disable = false;
    
    // Select the right mode values
    switch(newMode){
        case cartesianStraight:
            pos = CARTESIAN_STRAIGHT_POSITION;
            code = CARTESIAN_STRAIGHT_MODE;
            break;

        case cartesian90degrees:
            pos = CARTESIAN_90DEGREES_POSITION;
            code = CARTESIAN_90DEGREES_MODE;
            break;

        case cylindricalStraight:
            pos = CYLINDRICAL_STRAIGHT_POSITION;
            code = CYLINDRICAL_STRAIGHT_MODE;
            break;

        case cylindrical90degrees:
            pos = CYLINDRICAL_90DEGREES_POSITION;
            code = CYLINDRICAL_90DEGREES_MODE;
            break;

        case backhoe:
            pos = BACKHOE_POSITION;
            code = BACKHOE_MODE;
            break;

        case sleeping:
        default:
            pos = SLEEP_POSITION;
            code = SLEEP_MODE;
            disable = true;
    }

    // Send the new position to the board
    std::vector<uint8_t> posCodes(NB_BIG_VALUE_MOTORS * 2 + NB_SMALL_VALUE_MOTORS, 0);

    posCodes.push_back(ENABLE_OUTPUT);
    posCodes.push_back(code);

    this->send(posCodes);

    std::vector<uint8_t> response;
    this->receive(response);


    // Set the new position
    if(!disable)
        this->forceMove(pos);
}



/**
 * @brief Convert a string into a mode
 * 
 * @param mode the string to convert
 * @return Mode the corresponding mode
 * 
 * If the string does not match a mode, returns sleeping mode by default
 */
Mode WidowX::stringToMode(const std::string& mode){
    if(mode == "cartesianStraight") return cartesianStraight;
    else if(mode == "cartesian90degrees") return cartesian90degrees;
    else if(mode == "cartesian90degrees") return cartesian90degrees;
    else if(mode == "cylindricalStraight") return cylindricalStraight;
    else if(mode == "cylindrical90degrees") return cylindrical90degrees;
    else if(mode == "backhoe") return backhoe;
    else return sleeping;
}

/**
 * @brief Convert a mode into a string
 * 
 * @param mode the mode to convert
 * @return std::string the corresponding string
 */
std::string WidowX::modeToString(const Mode mode){
    switch(mode){
        case cartesianStraight:
            return "cartesianStraight";
            break;
        
        case cartesian90degrees:
            return "cartesian90degrees";
            break;
        
        case cylindricalStraight:
            return "cylindricalStraight";
            break;
        
        case cylindrical90degrees:
            return "cylindrical90degrees";
            break;
        
        case backhoe:
            return "backhoe";
            break;
        
        case sleeping:
        default:
            return "sleeping";
            
    }
}
    
    
    
/**
 * @file definitions.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief 
 * @version 0.1
 * @date 2019-05-22
 * 
 * Most of the references for the values below can be found at https://learn.trossenrobotics.com/arbotix/arbotix-communication-controllers/31-arm-link-reference.html
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H


// Standard baudrate for WidowX communication
#define STANDARD_BAUDRATE 38400

// Valid header for a packet
#define HEADER 255
// Code for enabling output
#define ENABLE_OUTPUT 0

// Code for an ID request
#define CONNECT_REQUEST 112
// Minimum size of a packet
#define MIN_PACKET_SIZE 5
// ID corresponding to the WidowX arm
#define VALID_ID 3

// Maximum delay to attempt to connect to the board (in ms)
#define CONNECT_TIMEOUT 10000

// Number of motors with range requiring 2 bytes
#define NB_BIG_VALUE_MOTORS 6
// Delta + number of motors with range requiring 1 byte
#define NB_SMALL_VALUE_MOTORS 1


// Codes of the different modes
#define CARTESIAN_STRAIGHT_MODE 32
#define CARTESIAN_90DEGREES_MODE 40
#define CYLINDRICAL_STRAIGHT_MODE 48
#define CYLINDRICAL_90DEGREES_MODE 56
#define BACKHOE_MODE 64

/**
 * @brief Enum listing all the working arm modes
 * 
 */
enum Mode{
    cartesianStraight,
    cartesian90degrees,
    cylindricalStraight,
    cylindrical90degrees,
    backhoe,
    sleeping
};

#define MOVE_MODE 0 // Used to send coordinates to the device, the coordinate system changes according to the current mode used (see list above)
#define SLEEP_MODE 96 // Used to move th device back to its initial position

// Coordinates of the default positions of each mode
#define CARTESIAN_STRAIGHT_POSITION {512, 250, 225, 90, 512, 256}
#define CARTESIAN_90DEGREES_POSITION {512, 150, 30, 0, 512, 256}
#define CYLINDRICAL_STRAIGHT_POSITION {2048, 250, 225, 90, 512, 256}
#define CYLINDRICAL_90DEGREES_POSITION {2048, 150, 30, 0, 512, 256}
#define BACKHOE_POSITION {2048, 2048, 2048, 2048, 512, 256}
#define SLEEP_POSITION {2048, 1024, 1024, 1789, 512, 256} // In backhoe definition, according to the mode will be set at different positions, do not use!

// Default motor speed value
#define DEFAULT_SPEED 205


#endif
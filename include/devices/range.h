/**
 * @file range.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief File containing the values for the servomotor ranges
 * @version 0.1
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef RANGE_H
#define RANGE_H


// Ranges of values for servomotor position with base type
#define BASE_MIN 0
#define BASE_MAX 4095

// Ranges of values for servomotor position with shoulder type
#define SHOULDER_MIN 1024
#define SHOULDER_MAX 3072

// Ranges of values for servomotor position with elbow type
#define ELBOW_MIN 1024
#define ELBOW_MAX 3072

// Ranges of values for servomotor position with wrist angle type
#define WRISTANGLE_MIN 1024
#define WRISTANGLE_MAX 3072

// Ranges of values for servomotor position with wrist rotate type
#define WRISTROTATE_MIN 0
#define WRISTROTATE_MAX 1023

// Ranges of values for servomotor position with gripper type
#define GRIPPER_MIN 0
#define GRIPPER_MAX 512

// Ranges of values for speed of servomotors, no matter their type
#define SPEED_MIN 0
#define SPEED_MAX 1023


#endif
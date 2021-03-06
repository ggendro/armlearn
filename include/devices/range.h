/**
 * @file range.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing the values for the servomotor ranges
 * @version 0.1
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */



#ifndef RANGE_H
#define RANGE_H

#include <math.h>

namespace armlearn {
    namespace communication{

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

// Unit of servomotor distances (0.088°)
#define MOVE_UNIT (360.0 / 4096.0)

// Convert a position in servomotor unit into degree (unit is MOVE_UNIT degrees)
#define TO_DEGREE(position) ((position) * MOVE_UNIT - 180)
// Convert a position in servomotor unit into radian (unit is MOVE_UNIT degrees)
#define TO_RADIAN(position) ((TO_DEGREE(position)) * M_PI / 180)

// Convert a position in degree into servomotor unit
#define FROM_DEGREE(position) (((position) + 180) / MOVE_UNIT)
// Convert a position in radian into servomotor unit
#define FROM_RADIAN(position) (FROM_DEGREE((position) / M_PI * 180))

// Unit of servomotor speed (0.114rpm)
#define SPEED_UNIT 0.114

    }
}


#endif
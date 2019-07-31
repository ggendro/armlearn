/**
 * @file learnsettings.h
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief File containing settings for learning
 * @version 0.1
 * @date 2019-07-17
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef LEARNSETTINGS_H
#define LEARNSETTINGS_H

// Coefficient of target error (difference between the real output and the target output, to minimize) when computing error between input and output
#define TARGET_COEFF 0.0168
// Coefficient of movement error (distance browsed by servomotors to reach target, to minimize) when computing error between input and output
#define MOVEMENT_COEFF 0.00016
// Coefficient of valid position error (returned if position is not valid)
#define VALID_COEFF -10

// Margin of error allowing to stop learning if error is below the number
#define LEARN_ERROR_MARGIN 0.05

// Number of iterations of the learning
#define LEARN_NB_ITERATIONS 25000
// Number of movents allowed to reach target during learning
#define LEARN_NB_MOVEMENTS 150
// Number of invalid movements allowed during one iteration
#define MAX_NULL_MOVE 0

// Coefficient decreasing the reward for a state as the state is closer to the initial state
#define DECREASING_REWARD 0.99

#endif
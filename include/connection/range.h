/**
 * @file range.h
 * @author Gaël Gendron (gael.genron@insa-rennes.fr)
 * @brief Range values for servomotors
 * @version 0.1
 * @date 2019-05-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */

// TODO: (mode management) add values for other modes, change setUp function

#ifndef RANGE_H
#define RANGE_H

#include <string>
#include <vector>

#include "definitions.h"


#define BASE_MIN_VALUE_BACKHOE 0
#define BASE_MAX_VALUE_BACKHOE 4095

#define SHOULDER_MIN_VALUE_BACKHOE 1024
#define SHOULDER_MAX_VALUE_BACKHOE 3072

#define ELBOW_MIN_VALUE_BACKHOE 1024
#define ELBOW_MAX_VALUE_BACKHOE 3072

#define WRIST_ANGLE_MIN_VALUE_BACKHOE 1024
#define WRIST_ANGLE_MAX_VALUE_BACKHOE 3072

#define WRIST_ROTATE_MIN_VALUE_BACKHOE 0
#define WRIST_ROTATE_MAX_VALUE_BACKHOE 1023

#define GRIPPER_MIN_VALUE_BACKHOE 0
#define GRIPPER_MAX_VALUE_BACKHOE 512

#define DELTA_MIN_VALUE_BACKHOE 0
#define DELTA_MAX_VALUE_BACKHOE 255


/**
 * @brief Range class for servomotors
 * 
 * Ranges for backhoe mode:
 * 
 * Base            [0, 4095]
 * Shoulder        [1024, 3072]
 * Elbow           [1024, 3072]
 * Wrist angle     [1024, 3072]
 * Wrist rotate    [0, 1023]
 * Gripper         [0, 512]
 * Delta           [0, 255]
 */
class Range{

    private:
        int minValue;
        int maxValue;
        std::vector<Mode>* domains;
        std::string motorName;

    public:
        Range(int min, int max, Mode dom, std::string& name);
        virtual ~Range();

        bool addDomain(Mode domain);
        bool removeDomain(Mode domain);

        bool isValid(int value) const;
        bool hasDomain(Mode domain) const;
        std::string getName() const;

        static std::vector<Range*>* buildWidowXSetup();

};

#endif
/**
 * @file test_converters.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of converter classes
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>
#include <iostream>

#include "optimcartesianconverter.h"
#include "basiccartesianconverter.h"
#include "cylindricalconverter.h"

class OptimCartesianConverterTest : public ::testing::Test {
    protected:

    OptimCartesianConverterTest() {
    }

    ~OptimCartesianConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("servo1", armlearn::kinematics::rotX, 0, 0, 74);
        converterFilled.addServo("servo2", armlearn::kinematics::rotZ, 0, 0, 41);
        converterFilled.addServo("servo3", armlearn::kinematics::fixed, 5, 4, 0);
        converterFilled.addServo("servo4", armlearn::kinematics::rotZ, 0, 0, 41);
    }

    void TearDown() override {
    }

    armlearn::kinematics::OptimCartesianConverter converterFilled;
    armlearn::kinematics::OptimCartesianConverter converterEmpty;
};

// Tests that the converter contains 3 servomotors
TEST_F(OptimCartesianConverterTest, checkNbServos) {
    ASSERT_EQ(converterFilled.getNbServos(), 3);
}



class BasicCartesianConverterTest : public ::testing::Test {
    protected:

    BasicCartesianConverterTest() {
    }

    ~BasicCartesianConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("servo1", armlearn::kinematics::rotX, 0, 0, 74);
        converterFilled.addServo("servo2", armlearn::kinematics::rotZ, 0, 0, 41);
        converterFilled.addServo("servo3", armlearn::kinematics::fixed, 5, 4, 0);
        converterFilled.addServo("servo4", armlearn::kinematics::rotZ, 0, 0, 41);
    }

    void TearDown() override {
    }

    armlearn::kinematics::BasicCartesianConverter converterFilled;
    armlearn::kinematics::BasicCartesianConverter converterEmpty;
};

// Tests that the converter contains 3 servomotors
TEST_F(BasicCartesianConverterTest, checkNbServos) {
    ASSERT_EQ(converterFilled.getNbServos(), 3);
}



class CylindricalConverterTest : public ::testing::Test {
    protected:

    CylindricalConverterTest() {
    }

    ~CylindricalConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("servo1", armlearn::kinematics::rotX, 0, 0, 74);
        converterFilled.addServo("servo2", armlearn::kinematics::rotZ, 0, 0, 41);
        converterFilled.addServo("servo3", armlearn::kinematics::fixed, 5, 4, 0);
        converterFilled.addServo("servo4", armlearn::kinematics::rotZ, 0, 0, 41);
    }

    void TearDown() override {
    }

    armlearn::kinematics::CylindricalConverter converterFilled;
    armlearn::kinematics::CylindricalConverter converterEmpty;
};

// Tests that the converter contains 3 servomotors
TEST_F(CylindricalConverterTest, checkNbServos) {
    ASSERT_EQ(converterFilled.getNbServos(), 3);
}






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
#include "convertererror.h"

class OptimCartesianConverterTest : public ::testing::Test {
    protected:

    OptimCartesianConverterTest() {
    }

    ~OptimCartesianConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("base", armlearn::kinematics::rotZ, 0, 0, 125, 0, 0, M_PI);
        converterFilled.addServo("shoulder", armlearn::kinematics::rotX, 0, -49, 142, M_PI/2, 0, M_PI);
        converterFilled.addServo("elbow", armlearn::kinematics::fixed, 0, 0, 71);
        converterFilled.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 71);
    }

    void TearDown() override {
    }

    armlearn::kinematics::OptimCartesianConverter converterFilled;
};

// Tests that the converter contains 3 servomotors
TEST_F(OptimCartesianConverterTest, checkNbServos) {
    ASSERT_EQ(converterFilled.getNbServos(), 3);
}

// Tests that the converter can remove its servos
TEST_F(OptimCartesianConverterTest, removeNbServos) {
    converterFilled.removeAllServos();
    ASSERT_EQ(converterFilled.getNbServos(), 0);
}

// Tests that the converter can compute forward kinematics
TEST_F(OptimCartesianConverterTest, forwardKinematics) {
    auto pos = converterFilled.computeServoToCoord({2048, 2048, 2048})->getCoord();
    auto rep = {0, 191, 267};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}

// Tests that the converter can compute inverse kinematics
TEST_F(OptimCartesianConverterTest, inverseKinematics) { // TODO: IK cannot be computed yet, make it work
    auto pos = converterFilled.computeCoordToServo({0, 197, 267})->getServo();
    auto rep = {2048, 2048, 2048};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}



class BasicCartesianConverterTest : public ::testing::Test {
    protected:

    BasicCartesianConverterTest() {
    }

    ~BasicCartesianConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("base", armlearn::kinematics::rotZ, 0, 0, 125, 0, 0, M_PI);
        converterFilled.addServo("shoulder", armlearn::kinematics::rotX, 0, -49, 142, M_PI/2, 0, M_PI);
        converterFilled.addServo("elbow", armlearn::kinematics::fixed, 0, 0, 71);
        converterFilled.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 71);
    }

    void TearDown() override {
    }

    armlearn::kinematics::BasicCartesianConverter converterFilled;
};

// Tests that the converter contains 3 servomotors
TEST_F(BasicCartesianConverterTest, checkNbServos) {
    ASSERT_EQ(converterFilled.getNbServos(), 3);
}

// Tests that the converter can remove its servos
TEST_F(BasicCartesianConverterTest, removeNbServos) {
    converterFilled.removeAllServos();
    ASSERT_EQ(converterFilled.getNbServos(), 0);
}

// Tests that the converter can compute forward kinematics
TEST_F(BasicCartesianConverterTest, forwardKinematics) {
    auto pos = converterFilled.computeServoToCoord({2048, 2048, 2048})->getCoord();
    auto rep = {0, 191, 267};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}

// Tests that the converter can compute inverse kinematics
TEST_F(BasicCartesianConverterTest, inverseKinematics) { // TODO: IK cannot be computed yet, make it work
    auto pos = converterFilled.computeCoordToServo({0, 197, 267})->getServo();
    auto rep = {2048, 2048, 2048};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}



class CylindricalConverterTest : public ::testing::Test {
    protected:

    CylindricalConverterTest() {
    }

    ~CylindricalConverterTest() override {
    }

    void SetUp() override {
        converterFilled.addServo("base", armlearn::kinematics::rotZ, 0, 0, 125, 0, 0, M_PI);
        converterFilled.addServo("shoulder", armlearn::kinematics::rotX, 0, -49, 142, M_PI/2, 0, M_PI);
        converterFilled.addServo("elbow", armlearn::kinematics::fixed, 0, 0, 71);
        converterFilled.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 71);
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

// Tests that the converter can remove its servos
TEST_F(CylindricalConverterTest, removeNbServos) {
    converterFilled.removeAllServos();
    ASSERT_EQ(converterFilled.getNbServos(), 0);
}

// Tests that the converter can compute forward kinematics
TEST_F(CylindricalConverterTest, forwardKinematics) {
    auto pos = converterFilled.computeServoToCoord({2048, 2048, 2048})->getCoord();
    std::initializer_list<double> rep = {191, M_PI/2, 267};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}

// Tests that the converter can compute inverse kinematics
TEST_F(CylindricalConverterTest, inverseKinematics) { // TODO: IK cannot be computed yet, make it work
    auto pos = converterFilled.computeCoordToServo({0, 197, 267})->getServo();
    auto rep = {2048, 2048, 2048};

    ASSERT_EQ(pos.size(), 3);

    auto verifPtr = pos.cbegin();
    for(auto& v : rep) {
        ASSERT_NEAR(v, *verifPtr, 1);
        verifPtr++;
    }
}

// Tests exception throw when arm does not have a cylindrical base
TEST_F(CylindricalConverterTest, noBaseExcept) {
    ASSERT_THROW(converterEmpty.addServo("notPossibleMotor", armlearn::kinematics::transX, 0, 0, 71), armlearn::ConverterError);
}






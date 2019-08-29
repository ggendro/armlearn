/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of WidowXBuilder class
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "widowxbuilder.h"
#include "nowaitarmsimulator.h"
#include "cylindricalconverter.h"


class WidowXBuilderTest : public ::testing::Test {
    protected:

    WidowXBuilderTest() {
    }

    ~WidowXBuilderTest() override {
    }

    void SetUp() override {
        builder.buildController(controller);
        builder.buildConverter(converter);
    }

    void TearDown() override {
    }

    armlearn::WidowXBuilder builder;
    armlearn::communication::NoWaitArmSimulator controller;
    armlearn::kinematics::CylindricalConverter converter;
};


// Tests that the controller contains ID 1
TEST_F(WidowXBuilderTest, verifyID1) {
    ASSERT_TRUE(controller.changeId(1, 8));
}

// Tests that the controller contains ID 2
TEST_F(WidowXBuilderTest, verifyID2) {
    ASSERT_TRUE(controller.changeId(2, 8));
}

// Tests that the controller contains ID 3
TEST_F(WidowXBuilderTest, verifyID3) {
    ASSERT_TRUE(controller.changeId(3, 8));
}

// Tests that the controller contains ID 4
TEST_F(WidowXBuilderTest, verifyID4) {
    ASSERT_TRUE(controller.changeId(4, 8));
}

// Tests that the controller contains ID 5
TEST_F(WidowXBuilderTest, verifyID5) {
    ASSERT_TRUE(controller.changeId(5, 8));
}

// Tests that the controller contains ID 6
TEST_F(WidowXBuilderTest, verifyID16) {
    ASSERT_TRUE(controller.changeId(6, 8));
}

// Tests that the controller does not contain ID 7
TEST_F(WidowXBuilderTest, verifyID7) {
    ASSERT_FALSE(controller.changeId(7, 8));
}

// Tests that the converter contains 6 servomotors
TEST_F(WidowXBuilderTest, verifyConverter) {
    ASSERT_EQ(converter.getNbServos(), 6);
}





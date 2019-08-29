/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of simulator classes
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "armsimulator.h"


class ArmSimulatorTest : public ::testing::Test {
    protected:

    ArmSimulatorTest() {
    }

    ~ArmSimulatorTest() override {
    }

    void SetUp() override {
        sim.addMotor(1, "base ", armlearn::communication::base);
        sim.addMotor(2, "shoulder", armlearn::communication::shoulder);
        sim.addMotor(3, "elbow", armlearn::communication::elbow);
    }

    void TearDown() override {
    }

    armlearn::communication::ArmSimulator sim;
};


// Tests that the controller contains ID 1
TEST_F(ArmSimulatorTest, verifyID1) {
    ASSERT_TRUE(sim.changeId(1, 4));
}





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

#include <armlearn/widowxbuilder.h>
#include <armlearn/nowaitarmsimulator.h>
#include <armlearn/cylindricalconverter.h>


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


// Tests that the controller contains 6 and only 6 servomotors with IDs fom 1 to 6
TEST_F(WidowXBuilderTest, verifyController) {
    ASSERT_TRUE(controller.changeId(1, 1));
    ASSERT_TRUE(controller.changeId(2, 2));
    ASSERT_TRUE(controller.changeId(3, 3));
    ASSERT_TRUE(controller.changeId(4, 4));
    ASSERT_TRUE(controller.changeId(5, 5));
    ASSERT_TRUE(controller.changeId(6, 6));
    ASSERT_FALSE(controller.changeId(7, 7));
}

// Tests that the onverter contains 6 servomotors
TEST_F(WidowXBuilderTest, verifyConverter) {
    ASSERT_EQ(converter.getNbServos(), 6);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}// Build command: g++ xxx.cpp -lgtest -lpthread


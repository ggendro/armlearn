/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of SerialController class
 * @version 0.1
 * @date 2019-08-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "serialcontroller.h"
#include "widowxbuilder.h"


class SerialControllerTest : public ::testing::Test { // Tests can only be executed if a WidowX robotic arm is connected to the serial port
    protected:

    SerialControllerTest() {
        arbotix = new armlearn::communication::SerialController("/dev/ttyUSB0", armlearn::communication::except);

        servos.push_back(arbotix->showServomotor(1));
        servos.push_back(arbotix->showServomotor(2));
        servos.push_back(arbotix->showServomotor(3));
        servos.push_back(arbotix->showServomotor(4));
        servos.push_back(arbotix->showServomotor(5));
        servos.push_back(arbotix->showServomotor(6));
    }

    ~SerialControllerTest() override {
    }

    void SetUp() override {
        armlearn::WidowXBuilder builder;
        builder.buildController(*arbotix);
    }

    void TearDown() override {
    }

    armlearn::communication::AbstractController* arbotix;
    std::vector<const armlearn::communication::Servomotor*> servos;
};


// Tests connection
TEST_F(SerialControllerTest, checkConnection) { //TODO: create tests that can check the good behavior of the code without requiring sudo access, maybe use googleMock libray
    arbotix->connect();
    for(auto& s : servos) ASSERT_EQ(s->getStatus(), armlearn::communication::activated);
}
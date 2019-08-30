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

#include "nowaitarmsimulator.h"
#include "servomotor.h"
#include "iderror.h"
#include "outofrangeerror.h"


class ArmSimulatorTest : public ::testing::Test {
    protected:

    ArmSimulatorTest() {
        sim = new armlearn::communication::ArmSimulator(armlearn::communication::except);
        noWaitSim = new armlearn::communication::NoWaitArmSimulator(armlearn::communication::except);
    }

    ~ArmSimulatorTest() override {
    }

    void SetUp() override {
        sim->addMotor(1, "base ", armlearn::communication::base);
        sim->addMotor(2, "shoulder", armlearn::communication::shoulder);
        sim->addMotor(3, "elbow", armlearn::communication::elbow);

        servos.push_back(sim->showServomotor(1));
        servos.push_back(sim->showServomotor(2));
        servos.push_back(sim->showServomotor(3));

        noWaitSim->addMotor(1, "base ", armlearn::communication::base);
        noWaitSim->addMotor(2, "shoulder", armlearn::communication::shoulder);
        noWaitSim->addMotor(3, "elbow", armlearn::communication::elbow);

        noWaitServos.push_back(noWaitSim->showServomotor(1));
        noWaitServos.push_back(noWaitSim->showServomotor(2));
        noWaitServos.push_back(noWaitSim->showServomotor(3));
    }

    void TearDown() override {
    }

    armlearn::communication::AbstractController* sim;
    armlearn::communication::AbstractController* noWaitSim;

    std::vector<const armlearn::communication::Servomotor*> servos;
    std::vector<const armlearn::communication::Servomotor*> noWaitServos;
};


// Tests connection
TEST_F(ArmSimulatorTest, checkConnection) {
    sim->connect();
    for(auto& s : servos) ASSERT_EQ(s->getStatus(), armlearn::communication::activated);
}

// Tests that the controller contains ID 1
TEST_F(ArmSimulatorTest, verifyID1) {
    ASSERT_TRUE(sim->changeId(1, 4));
}

// Tests that the controller contains ID 2
TEST_F(ArmSimulatorTest, verifyID2) {
    ASSERT_TRUE(sim->changeId(2, 4));
}

// Tests that the controller is able to remove a motor
TEST_F(ArmSimulatorTest, verifyServo1) {
    sim->removeMotor(2);
    ASSERT_FALSE(sim->changeId(2, 4));
}

// Tests exception when removing incorrect motor
TEST_F(ArmSimulatorTest, exceptRemove) {
    ASSERT_THROW(sim->removeMotor(4), armlearn::IdError);
}

// Tests the modification of the display mode
TEST_F(ArmSimulatorTest, setDisplay) {
    sim->setDisplayMode(armlearn::communication::none);
    ASSERT_EQ(sim->getDisplayMode(), 0);
}

// Tests that a motor with ID already taken cannot be added
TEST_F(ArmSimulatorTest, addExcept) {
    ASSERT_THROW(sim->addMotor(3, "elbow", armlearn::communication::elbow), armlearn::IdError);
}

// Tests that setting display mode to 0 disables exceptions
TEST_F(ArmSimulatorTest, addNoExcept) {
    sim->setDisplayMode(armlearn::communication::none);
    ASSERT_NO_THROW(sim->addMotor(3, "elbow", armlearn::communication::elbow));
}

// Tests the well behavior of changeId method
TEST_F(ArmSimulatorTest, changeId) {
    sim->changeId(1, 4);
    ASSERT_EQ(servos[0]->getId(), 4);
}

// Tests the well behavior of turnLED method without arguments
TEST_F(ArmSimulatorTest, turnLEDnoArgs) {
    bool on = servos[0]->getLED();
    sim->turnLED(1);
    ASSERT_TRUE(on != servos[0]->getLED());
}

// Tests the well behavior of turnLED method with arguments
TEST_F(ArmSimulatorTest, turnLEDargs) {
    bool on = servos[0]->getLED();
    sim->turnLED(1, !on);
    ASSERT_TRUE(on != servos[0]->getLED());
}

// Tests the well behavior of enableTorque method, enabling
TEST_F(ArmSimulatorTest, enableTorque) {
    sim->enableTorque(1, true);
    ASSERT_TRUE(sim->torqueEnabled(1));
}

// Tests the well behavior of enableTorque method, disabling
TEST_F(ArmSimulatorTest, disableTorque) {
    sim->enableTorque(1, false);
    ASSERT_FALSE(sim->torqueEnabled(1));
}

// Tests the well behavior of changeSpeed method for all servomotors
TEST_F(ArmSimulatorTest, changeSpeed) {
    sim->changeSpeed(27);
    for(auto& s : servos) ASSERT_EQ(s->getTargetSpeed(), 27);
}

// Tests exceptions when invalid parameters for changeSpeed method
TEST_F(ArmSimulatorTest, exceptChangeSpeed) {
    ASSERT_THROW(sim->changeSpeed(1025), armlearn::OutOfRangeError);
}

// Tests the well behavior of setPosition method for all servomotors, verification done for target position
TEST_F(ArmSimulatorTest, setTargetPos) {
    std::vector<uint16_t> pos = {2000, 1700, 2900};
    sim->setPosition(pos);

    auto ptr = pos.cbegin();
    for(auto& s : servos) {
        ASSERT_EQ(s->getTargetPosition(), *ptr);
        ptr++;
    }
}

// Tests range exception for setPosition method
TEST_F(ArmSimulatorTest, setTargetPosExcept) {
    ASSERT_THROW(sim->setPosition(1, 4097), armlearn::OutOfRangeError);
}

// Tests id exception for setPosition method
TEST_F(ArmSimulatorTest, setTargetIdExcept) {
    ASSERT_THROW(sim->setPosition(4, 2048), armlearn::IdError);
}

// Tests the well behavior of setPosition method for all servomotors, verification done for current position
TEST_F(ArmSimulatorTest, setPos) {
    std::vector<uint16_t> pos = {2000, 1700, 2900};
    sim->changeSpeed(50);
    sim->setPosition(pos);
    sim->waitFeedback();

    auto ptr = pos.cbegin();
    for(auto& s : servos) {
        ASSERT_EQ(s->getCurrentPosition(), *ptr);
        ptr++;
    }
}

// Tests the well behavior of setPosition method for all servomotors, verification done for current position of noWait Simulator
TEST_F(ArmSimulatorTest, noWaitSetPos) {
    std::vector<uint16_t> pos = {2000, 1700, 2900};
    noWaitSim->setPosition(pos);
    noWaitSim->waitFeedback();

    auto ptr = pos.cbegin();
    for(auto& s : noWaitServos) {
        ASSERT_EQ(s->getCurrentPosition(), *ptr);
        ptr++;
    }
}

// Tests the well behavior of addPosition method for all servomotors, verification done for current position, from backhoe
TEST_F(ArmSimulatorTest, addPosBackhoe) {
    sim->changeSpeed(50);
    sim->goToBackhoe();
    sim->waitFeedback();

    std::vector<uint16_t> curPos = sim->getPosition();
    std::vector<int> pos = {1, -12, 24};
    sim->addPosition(pos);
    sim->waitFeedback();

    auto ptr = pos.cbegin();
    auto curPtr = curPos.cbegin();
    for(auto& s : servos) {
        ASSERT_EQ(s->getCurrentPosition(), *curPtr+*ptr);
        ptr++;
        curPtr++;
    }
}

// Tests the well behavior of addPosition method for all servomotors, verification done for current position, from sleep
TEST_F(ArmSimulatorTest, addPosSleep) {
    sim->changeSpeed(50);
    sim->goToSleep();
    sim->waitFeedback();

    std::vector<uint16_t> curPos = sim->getPosition();
    std::vector<int> pos = {-200, 102, 24};
    sim->addPosition(pos);
    sim->waitFeedback();

    auto ptr = pos.cbegin();
    auto curPtr = curPos.cbegin();
    for(auto& s : servos) {
        ASSERT_EQ(s->getCurrentPosition(), *curPtr+*ptr);
        ptr++;
        curPtr++;
    }
}

// Tests the well behavior of addPosition method for all servomotors, verification done for current position of noWait simulator
TEST_F(ArmSimulatorTest, noWaitAddPos) {
    noWaitSim->goToBackhoe();
    noWaitSim->waitFeedback();

    std::vector<uint16_t> curPos = noWaitSim->getPosition();
    std::vector<int> pos = {1, -12, 24};
    noWaitSim->addPosition(pos);
    noWaitSim->waitFeedback();

    auto ptr = pos.cbegin();
    auto curPtr = curPos.cbegin();
    for(auto& s : noWaitServos) {
        ASSERT_EQ(s->getCurrentPosition(), *curPtr+*ptr);
        ptr++;
        curPtr++;
    }
}

// Tests if valid position is valid
TEST_F(ArmSimulatorTest, validPos) {
    ASSERT_TRUE(sim->validPosition({2048, 2048, 2048}));
}

// Tests if position with incorrect dimension is valid
TEST_F(ArmSimulatorTest, invalidPosSize) {
    ASSERT_FALSE(sim->validPosition({2048, 2048}));
}

// Tests if position with vales out of range is valid
TEST_F(ArmSimulatorTest, invalidPosRange) {
    ASSERT_FALSE(sim->validPosition({0, 5012, 2048}));
}




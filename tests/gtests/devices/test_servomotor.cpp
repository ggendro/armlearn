/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of simulator classes
 * @version 0.1
 * @date 2019-08-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "servomotor.h"
#include "typeerror.h"


class ServomotorTest : public ::testing::Test {
    protected:

    ServomotorTest() {
        id=1;
        name = "servo";
        type = armlearn::communication::base;
        servo = new armlearn::communication::Servomotor(id, name, type);
    }

    ~ServomotorTest() override {
    }

    void SetUp() override {
    }

    void TearDown() override {
    }

    int id;
    std::string name;
    armlearn::communication::Type type;
    armlearn::communication::Servomotor* servo;
};


// Test getName
TEST_F(ServomotorTest, getName) {
    ASSERT_EQ(servo->getName(), name);
}

// Test getId
TEST_F(ServomotorTest, getId) {
    ASSERT_EQ(servo->getId(), id);
}

// Test getType
TEST_F(ServomotorTest, getType) {
    ASSERT_EQ(servo->getType(), type);
}

// Test setName
TEST_F(ServomotorTest, setName) {
    std::string newName = "newServo";
    servo->setName(newName);
    ASSERT_EQ(servo->getName(), newName);
}

// Test setId
TEST_F(ServomotorTest, setId) {
    int newId = 2;
    servo->setId(newId);
    ASSERT_EQ(servo->getId(), newId);
}

// Test setType
TEST_F(ServomotorTest, setType) {
    armlearn::communication::Type newType = armlearn::communication::gripper;
    servo->setType(newType);
    ASSERT_EQ(servo->getType(), newType);
}

// Test except setType
TEST_F(ServomotorTest, exceptSetType) {
    armlearn::communication::Type newType = (armlearn::communication::Type)12;
    ASSERT_THROW(servo->setType(newType), armlearn::TypeError);
}
/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of iodata classes
 * @version 0.1
 * @date 2019-08-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "iovector.h"
#include "input.h"
#include "output.h"


class IOVectorTest : public ::testing::Test {
    protected:

    IOVectorTest() {
        val = {1, 32, 45, 78};
        valCopy = {1, 32, 45, 78};
        
        vectFilled = new armlearn::IOVector<double>(val);
        vectFilledCopy = new armlearn::IOVector<double>(valCopy);
        vectEmpty = new armlearn::IOVector<double>();
    }

    ~IOVectorTest() override {
    }

    void SetUp() override {
    }

    void TearDown() override {
    }

    armlearn::IOVector<double>* vectFilled;
    armlearn::IOVector<double>* vectFilledCopy;
    armlearn::IOVector<double>* vectEmpty;

    std::vector<double> val;
    std::vector<double> valCopy;
};


// Tests that the IOvector returns its content
TEST_F(IOVectorTest, checkContent) {
    auto res = vectFilled->getVector();
    ASSERT_EQ(res.size(), val.size());

    auto verifPtr = res.cbegin();
    for(auto& v : val) {
        ASSERT_EQ(v, *verifPtr);
        verifPtr++;
    }
}

// Tests that the IOvector returns empty vector when null
TEST_F(IOVectorTest, checkEmptyContent) {
    ASSERT_EQ(vectEmpty->getVector().size(), 0);
}

// Tests that two IOvectors with same content are equals
TEST_F(IOVectorTest, checkEqual) {
    ASSERT_EQ(*vectFilled, *vectFilledCopy);
}

// Tests copy constructor
TEST_F(IOVectorTest, checkCopyCons) {
    ASSERT_EQ(*vectFilled, armlearn::IOVector<double>(*vectFilled));
}

// Tests copy
TEST_F(IOVectorTest, checkCopy) {
    auto copy = *vectFilled;
    ASSERT_EQ(*vectFilled, copy);
}



class InputTest : public ::testing::Test {
    protected:

    InputTest() {
        val = {1, 32, 45, 78};
        valCopy = {1, 32, 45, 78};
        
        vectFilled = new armlearn::Input<double>(val);
        vectFilledCopy = new armlearn::Input<double>(valCopy);
        vectEmpty = new armlearn::Input<double>();
    }

    ~InputTest() override {
    }

    void SetUp() override {
    }

    void TearDown() override {
    }

    armlearn::Input<double>* vectFilled;
    armlearn::Input<double>* vectFilledCopy;
    armlearn::Input<double>* vectEmpty;

    std::vector<double> val;
    std::vector<double> valCopy;
};


// Tests that the Input returns its content
TEST_F(InputTest, checkContent) {
    auto res = vectFilled->getInput();
    ASSERT_EQ(res.size(), val.size());

    auto verifPtr = res.cbegin();
    for(auto& v : val) {
        ASSERT_EQ(v, *verifPtr);
        verifPtr++;
    }
}

// Tests that the Input vector and input value are equal
TEST_F(InputTest, checkEqVec) {
    auto out = vectFilled->getInput();
    auto vec = vectFilled->getVector();
    ASSERT_EQ(out.size(), vec.size());

    auto verifPtr = out.cbegin();
    for(auto& v : vec) {
        ASSERT_EQ(v, *verifPtr);
        verifPtr++;
    }
}

// Tests that the Input returns empty vector when null
TEST_F(InputTest, checkEmptyContent) {
    ASSERT_EQ(vectEmpty->getInput().size(), 0);
}

// Tests that two Inputs with same content are equals
TEST_F(InputTest, checkEqual) {
    ASSERT_EQ(*vectFilled, *vectFilledCopy);
}

// Tests copy constructor
TEST_F(InputTest, checkCopyCons) {
    ASSERT_EQ(*vectFilled, armlearn::Input<double>(*vectFilled));
}

// Tests copy
TEST_F(InputTest, checkCopy) {
    auto copy = *vectFilled;
    ASSERT_EQ(*vectFilled, copy);
}



class OutputTest : public ::testing::Test {
    protected:

    OutputTest() {
        val = {1, 32, 45, 78};
        valCopy = {1, 32, 45, 78};
        
        vectFilled = new armlearn::Output<double>(val);
        vectFilledCopy = new armlearn::Output<double>(valCopy);
        vectEmpty = new armlearn::Output<double>();
    }

    ~OutputTest() override {
    }

    void SetUp() override {
    }

    void TearDown() override {
    }

    armlearn::Output<double>* vectFilled;
    armlearn::Output<double>* vectFilledCopy;
    armlearn::Output<double>* vectEmpty;

    std::vector<double> val;
    std::vector<double> valCopy;
};


// Tests that the Output returns its content
TEST_F(OutputTest, checkContent) {
    auto res = vectFilled->getOutput();
    ASSERT_EQ(res.size(), val.size());

    auto verifPtr = res.cbegin();
    for(auto& v : val) {
        ASSERT_EQ(v, *verifPtr);
        verifPtr++;
    }
}

// Tests that the Output vector and output value are equal
TEST_F(OutputTest, checkEqVec) {
    auto out = vectFilled->getOutput();
    auto vec = vectFilled->getVector();
    ASSERT_EQ(out.size(), vec.size());

    auto verifPtr = out.cbegin();
    for(auto& v : vec) {
        ASSERT_EQ(v, *verifPtr);
        verifPtr++;
    }
}

// Tests that the Output returns empty vector when null
TEST_F(OutputTest, checkEmptyContent) {
    ASSERT_EQ(vectEmpty->getOutput().size(), 0);
}

// Tests that two Outputs with same content are equals
TEST_F(OutputTest, checkEqual) {
    ASSERT_EQ(*vectFilled, *vectFilledCopy);
}

// Tests copy constructor
TEST_F(OutputTest, checkCopyCons) {
    ASSERT_EQ(*vectFilled, armlearn::Output<double>(*vectFilled));
}

// Tests copy
TEST_F(OutputTest, checkCopy) {
    auto copy = *vectFilled;
    ASSERT_EQ(*vectFilled, copy);
}
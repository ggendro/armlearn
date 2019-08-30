/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of exceptions classes
 * @version 0.1
 * @date 2019-08-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "computationerror.h"
#include "connectionerror.h"
#include "convertererror.h"
#include "fileerror.h"
#include "iderror.h"
#include "outofrangeerror.h"
#include "trajectoryerror.h"
#include "typeerror.h"


// Tests that exception message is correctly sent when reference constructor used
TEST(ComputationErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::ComputationError(test);
    }catch(armlearn::ComputationError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(ComputationErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::ComputationError(test);
    }catch(armlearn::ComputationError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(ConnectionErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::ConnectionError(test);
    }catch(armlearn::ConnectionError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(ConnectionErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::ConnectionError(test);
    }catch(armlearn::ConnectionError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(ConverterErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::ConverterError(test);
    }catch(armlearn::ConverterError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(ConverterErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::ConverterError(test);
    }catch(armlearn::ConverterError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(FileErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::FileError(test);
    }catch(armlearn::FileError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(FileErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::FileError(test);
    }catch(armlearn::FileError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(IdErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::IdError(test);
    }catch(armlearn::IdError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(IdErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::IdError(test);
    }catch(armlearn::IdError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(OutOfRangeErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::OutOfRangeError(test);
    }catch(armlearn::OutOfRangeError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(OutOfRangeErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::OutOfRangeError(test);
    }catch(armlearn::OutOfRangeError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(TrajectoryErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::TrajectoryError(test);
    }catch(armlearn::TrajectoryError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(TrajectoryErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::TrajectoryError(test);
    }catch(armlearn::TrajectoryError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}


// Tests that exception message is correctly sent when reference constructor used
TEST(TypeErrorTest, verifyRef) {
    std::string test = "test";
    try{
        throw armlearn::TypeError(test);
    }catch(armlearn::TypeError& e){
        ASSERT_EQ(e.what(), test);
    }
}

// Tests that exceptin message is correctly sent when pointer constructor used
TEST(TypeErrorTest, verifyPtr) {
    char test[4];
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';

    std::string strTest = "test";
    try{
        throw armlearn::TypeError(test);
    }catch(armlearn::TypeError& e){
        ASSERT_EQ(e.what(), strTest);
    }
}
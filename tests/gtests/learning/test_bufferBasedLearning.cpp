/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of BufferBasedPyLearner classes
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "setmodepylearner.h"
#include "addmodepylearner.h"
#include "optimcartesianconverter.h"
#include "nowaitarmsimulator.h"
#include "widowxbuilder.h"


class SetModePylearnerTest : public ::testing::Test {
    protected:

    armlearn::communication::AbstractController* sim;
    armlearn::kinematics::Converter* conv;
    armlearn::learning::DeviceLearner* learner;

    SetModePylearnerTest() {
        sim = new armlearn::communication::NoWaitArmSimulator();
        conv = new armlearn::kinematics::OptimCartesianConverter();
        learner = new armlearn::learning::SetModePyLearner(sim, conv, "../../../tests/gtests/learning/learntestparam.json");
    }

    ~SetModePylearnerTest() override {
        delete learner;
        delete conv;
        delete sim;
    }

    void SetUp() override {
        armlearn::WidowXBuilder builder;
        builder.buildController(*sim);
        builder.buildConverter(*conv);
    }

    void TearDown() override {
    }
};


// Tests that the learner is able to return a result from a trained model
TEST_F(SetModePylearnerTest, computeFromTrain) {
    std::vector<uint16_t> raw_dest = {5, 50, 300};
    auto dest = new armlearn::Input<uint16_t>(raw_dest);
    learner->addToLearningSet(dest, new armlearn::Output<std::vector<uint16_t>>());

    std::vector<double> res = conv->computeServoToCoord(*(learner->produce(*dest)->getOutput().rbegin()))->getCoord();
    
    ASSERT_EQ(res.size(), 3);

    auto verifPtr = res.cbegin();
    for(auto& v : raw_dest) {
        ASSERT_NEAR(v, *verifPtr, 50);
        verifPtr++;
    }
}





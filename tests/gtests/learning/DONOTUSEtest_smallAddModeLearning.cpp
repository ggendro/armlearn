/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of SmallAddModePyLeaner class
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "smalladdmodepylearner.h"
#include "optimcartesianconverter.h"
#include "nowaitarmsimulator.h"
#include "widowxbuilder.h"

// /!\ Cannot be used at the same time than another learning test --> requires access to the same files

class SmallAddModePylearnerTest : public ::testing::Test {
    protected:

    armlearn::communication::AbstractController* sim;
    armlearn::kinematics::Converter* conv;
    armlearn::learning::DeviceLearner* learner;

    SmallAddModePylearnerTest() {
        sim = new armlearn::communication::NoWaitArmSimulator(armlearn::communication::except);
        conv = new armlearn::kinematics::OptimCartesianConverter();
        learner = new armlearn::learning::SmallAddModePyLearner(sim, conv, "../../../tests/gtests/learning/learntestparam.json");
    }

    ~SmallAddModePylearnerTest() override {
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
TEST_F(SmallAddModePylearnerTest, computeFromTrain) {
    std::vector<uint16_t> raw_dest = {5, 50, 300};
    auto dest = new armlearn::Input<uint16_t>(raw_dest);
    learner->addToLearningSet(dest, new armlearn::Output<std::vector<uint16_t>>());
    
    sim->goToBackhoe(); // Reset position
	sim->waitFeedback();
    
    auto pos = sim->getPosition();
    auto res = *(learner->produce(*dest)->getOutput().rbegin());
    
    ASSERT_EQ(res.size(), 3);
    auto verifPtr = res.cbegin();
    for(auto& v : res) {
        ASSERT_NEAR(v, *verifPtr, 2);
        verifPtr++;
    }
}
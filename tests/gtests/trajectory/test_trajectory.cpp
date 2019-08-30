/**
 * @file test_builder.cpp
 * @author Gaël Gendron (gael.gendron@insa-rennes.fr)
 * @brief Testing file of Trajectory class
 * @version 0.1
 * @date 2019-08-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>

#include "trajectory.h"
#include "nowaitarmsimulator.h"


class TrajectoryTest : public ::testing::Test {
    protected:

    TrajectoryTest() {
        sim = new armlearn::communication::NoWaitArmSimulator(armlearn::communication::except);

        pathFilled = new armlearn::Trajectory(sim);
        pathEmpty = new armlearn::Trajectory(sim);
        pathErr = new armlearn::Trajectory(sim);
    }

    ~TrajectoryTest() override {
    }

    void SetUp() override {
        sim->addMotor(1, "base", armlearn::communication::base);
        sim->addMotor(2, "shoulder", armlearn::communication::shoulder);
        sim->addMotor(3, "elbow", armlearn::communication::elbow);

        initPoint = {2045, 1050, 3012};
        middlePoint = {1017, 2000, 2888};
        lastPoint = {3071, 2048, 1472};

        pathFilled->addPoint(initPoint);
        pathFilled->addPoint(middlePoint);
        pathFilled->addPoint({2046, 1027, 2027});
        pathFilled->addPoint({1041, 2001, 3009});
        pathFilled->addPoint(lastPoint);

        pathErr->addPoint({2048, 2048, 2048});
        pathErr->addPoint({0, 0, 6012});
        pathErr->addPoint(lastPoint);
    }

    void TearDown() override {
    }

    armlearn::communication::AbstractController* sim;

    armlearn::Trajectory* pathFilled;
    armlearn::Trajectory* pathEmpty;
    armlearn::Trajectory* pathErr;

    std::vector<uint16_t> initPoint;
    std::vector<uint16_t> middlePoint;
    std::vector<uint16_t> lastPoint;
};


// Tests that trajectory initializes correctly
TEST_F(TrajectoryTest, verifyInit) {
    pathFilled->init();

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), initPoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : initPoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}


// Tests that trajectory executes correctly
TEST_F(TrajectoryTest, verifyPath) {
    pathFilled->init();
    pathFilled->executeTrajectory();

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), lastPoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : lastPoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}

// Tests that error trajectory does not executes correctly
TEST_F(TrajectoryTest, exceptErrPath) {
    pathErr->init();
    ASSERT_THROW(pathErr->executeTrajectory(), armlearn::TrajectoryError);
}

// Tests that reverse trajectory executes correctly
TEST_F(TrajectoryTest, verifyReversePath) {
    pathFilled->init(true);
    pathFilled->executeTrajectory(true);

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), initPoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : initPoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}

// Tests that remove init point initializes to the next point
TEST_F(TrajectoryTest, verifyInitCut) {
    pathFilled->removePoint(0);

    pathFilled->init();

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), middlePoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : middlePoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}

// Tests that remove the last points executes the reduced trajectory
TEST_F(TrajectoryTest, verifyPathCut) {
    pathFilled->removePoint(2);
    pathFilled->removePoint(2);
    pathFilled->removePoint(2);

    pathFilled->init();
    pathFilled->executeTrajectory();

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), middlePoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : middlePoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}

// Tests that previous VerifyPathCut test also works when initalization done before cut
TEST_F(TrajectoryTest, verifyPathCutBeforeInit) {
    pathFilled->init();

    pathFilled->removePoint(2);
    pathFilled->removePoint(2);
    pathFilled->removePoint(2);

    pathFilled->executeTrajectory();

    auto pos = sim->getPosition();
    ASSERT_EQ(pos.size(), middlePoint.size());

    auto ptr = pos.cbegin();
    for(auto& s : middlePoint) {
        ASSERT_EQ(s, *ptr);
        ptr++;
    }
}

// Tests that initialization can be done when empty trajectory
TEST_F(TrajectoryTest, verifyEmptyInit) {
    ASSERT_NO_THROW(pathEmpty->init());
}

// Tests that adding point out of bounds throws exception
TEST_F(TrajectoryTest, exceptAdd) {
    ASSERT_THROW(pathEmpty->addPoint({2046, 1027, 2027}, 1), std::out_of_range);
}

// Tests that removing point out of bounds throws exception
TEST_F(TrajectoryTest, exceptRemove) {
    ASSERT_THROW(pathFilled->removePoint(7), std::out_of_range);
}

// Tests that removing point from empty trajectory throws exception
TEST_F(TrajectoryTest, exceptEmptyRemove) {
    ASSERT_THROW(pathEmpty->removePoint(),std::out_of_range);
}





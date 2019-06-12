
#include "armsimulator.h"
#include "serialcontroller.h"
#include "trajectory.h"

#include "simplelearner.h"
#include "widowxbuilder.h"
#include "optimcartesianconverter.h"


int main(int argc, char *argv[]) {

	OptimCartesianConverter conv;
	ArmSimulator arbotix;

	WidowXBuilder builder;
	builder.buildConverter(conv);
	builder.buildController(arbotix);


	SimpleLearner learner(&arbotix, &conv);

	SimpleInput dest({1, 3, 5});
	Output motorPositions = learner.produce(dest);

	std::cout << "Output : " << motorPositions.toString() << std::endl;

	/*
	learner.addToLearningSet(new Input(), new Output());
	learner.addToLearningSet(new Input(), new Output());
	learner.addToLearningSet(new Input(), new Output());

	learner.addToLearningSet(new SimpleInput({1, 4, 6}), new SimpleOutput({2, 3}));
	learner.addToLearningSet(new SimpleInput({0, 8, 9}), new SimpleOutput({58, 24, 2, 1}));
	learner.addToLearningSet(new SimpleInput({7, 8, 5}), new SimpleOutput({8, 45, }));
	learner.addToLearningSet(new SimpleInput({24, 56, 1}), new SimpleOutput({427, 24, 23, 13}));
	learner.addToLearningSet(new SimpleInput({3, 8, 45}), new SimpleOutput({0, 8, 2, 14}));
	learner.addToLearningSet(new SimpleInput({1, 4, 6}), new SimpleOutput({2, 3}));

	learner.generateTestingSet();

	std::cout << learner.toString();
	//*/



	/*
    //ArmSimulator arbotix;
	SerialController arbotix("/dev/ttyUSB0");

	arbotix.addMotor(1, "base ", base);
	arbotix.addMotor(2, "shoulder", shoulder);
	arbotix.addMotor(3, "elbow", elbow);
	arbotix.addMotor(4, "wristAngle", wristAngle);
	arbotix.addMotor(5, "wristRotate", wristRotate);
	arbotix.addMotor(6, "gripper", gripper);
	
	arbotix.connect();
	std::cout << arbotix.servosToString();

	arbotix.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();

    Trajectory path(&arbotix);

	path.addPoint(BACKHOE_POSITION);
	path.addPoint({1024, 2200, 2200, 1025, 512, 511});
	path.addPoint({1024, 2400, 2200, 1200, 512, 511});
	path.addPoint({1024, 2400, 2200, 1200, 512, 135});
	path.addPoint({1024, 2200, 2200, 1025, 512, 135});
	path.addPoint({2048, 2200, 2200, 1025, 512, 135});
	path.addPoint({2048, 2400, 2200, 1200, 512, 135});
	path.addPoint({2048, 2400, 2200, 1200, 512, 511});
	path.addPoint({2048, 2200, 2200, 1025, 512, 511});
	path.addPoint(SLEEP_POSITION);

	path.printTrajectory();
    
    // Variable stating the begin of calculations
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();


	path.init();
	path.executeTrajectory();

	// variable stating the end of calculations
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    std::cout << "Execution finished in " << std::chrono::duration<double, std::ratio<1, 1>>(endTime - startTime).count() << " s" << std::endl;
	//*/


}
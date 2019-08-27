
#include "nowaitarmsimulator.h"
#include "serialcontroller.h"
#include "trajectory.h"

#include "setmodepylearner.h"
#include "widowxbuilder.h"
#include "optimcartesianconverter.h"


int main(int argc, char *argv[]) {

	OptimCartesianConverter conv; // Create kinematics calculator
	NoWaitArmSimulator arbotix_sim((DisplayMode) 0); // Create robot simulator 
	SerialController arbotix("/dev/ttyUSB0"); // Create device connection

	WidowXBuilder builder;
	builder.buildConverter(conv);
	builder.buildController(arbotix);
	builder.buildController(arbotix_sim);

	arbotix.connect();

	std::this_thread::sleep_for((std::chrono::milliseconds) 1000); // Usually, a waiting period and a second connect attempt is necessary to reach all devices
	arbotix_sim.connect();
	arbotix.connect();

	arbotix_sim.changeSpeed(50);
	arbotix.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix_sim.updateInfos();
	arbotix.updateInfos();

	SetModePyLearner learner(&arbotix_sim, &conv); // Create learner

	auto targets = { // Inputs of learning, positions to ask to the robot
		new Input<uint16_t>({5, 50, 300}),
		new Input<uint16_t>({100, 100, 100})
	};
	for(auto& dest : targets) learner.addToLearningSet(dest, new Output<std::vector<uint16_t>>()); // Empty label for learning


	learner.learn(); // Execute learning
	std::cout << learner.toString();


	for(auto& dest : targets) { // Move robot to each position asked

		auto res = learner.produce(*dest);
		std::cout << "Input : " << dest->toString() << " - Output : " << res->toString() << std::endl;

		arbotix.goToBackhoe(); // Reset position
		arbotix.waitFeedback();

		auto moves = res->getOutput();
		for(auto ptr = moves.begin(); ptr < moves.end(); ptr++){
			arbotix.setPosition(*ptr);
			arbotix.waitFeedback();
		}

		delete res; // Do not forget
		
		std::cout << "Update servomotors information:" << std::endl;
		arbotix.updateInfos();

	}


}
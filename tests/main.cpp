
#include "nowaitarmsimulator.h"
#include "serialcontroller.h"
#include "trajectory.h"

#include "activepylearner.h"
#include "widowxbuilder.h"
#include "optimcartesianconverter.h"


int main(int argc, char *argv[]) {

	OptimCartesianConverter conv;
	NoWaitArmSimulator arbotix_sim((DisplayMode) 0);
	SerialController arbotix("/dev/ttyUSB0");

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

	ActivePyLearner learner(&arbotix, &conv);

	auto dest = new Input<uint16_t>({100, 100, 100});
	auto destMatch = new Output<uint16_t>();
	learner.addToLearningSet(dest, destMatch);
	learner.learn();

	auto res = learner.produce(*dest);
	std::cout << "Output : " << res->toString() << std::endl;

	arbotix.setPosition(res->getOutput());
	delete res; // Do not forget
	
	std::cout << "Update servomotors information:" << std::endl;
	arbotix.updateInfos();
	

	std::cout << learner.toString();

}
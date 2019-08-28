

#include "nowaitarmsimulator.h"
#include "serialcontroller.h"
#include "trajectory.h"

#include "setmodepylearner.h"
#include "widowxbuilder.h"
#include "optimcartesianconverter.h"

using namespace armlearn;

int main(int argc, char *argv[]) {

    /******************************************/
    /****          Learn example           ****/
    /******************************************/

    /*
     * Executes a learning algorithm on a simulation of a WidowX robot arm.
     * The learner learns how to reach the coordinates specified in input and returns a list of anular positions to get in order to reach the coordinates.
     * 
     */


    /******************************************************/
    /**** Creation and connection of device instances  ****/
    /******************************************************/

	kinematics::OptimCartesianConverter conv; // Create kinematics calculator
	communication::NoWaitArmSimulator arbotix_sim((communication::DisplayMode) 0); // Create robot simulator 

	WidowXBuilder builder;
	builder.buildConverter(conv);
	builder.buildController(arbotix_sim);

	arbotix_sim.connect();
	arbotix_sim.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix_sim.updateInfos();


    /******************************************************/
    /****        Creation or learning system           ****/
    /******************************************************/

	learning::SetModePyLearner learner(&arbotix_sim, &conv, "../../files/learnSettings/ddpglearnersettings.json"); // Create learner


    /******************************************************/
    /****           Learning on simulation             ****/
    /******************************************************/

	auto targets = { // Inputs of learning, positions to ask to the robot
		new Input<uint16_t>({5, 50, 300})
	};
	for(auto& dest : targets) learner.addToLearningSet(dest, new Output<std::vector<uint16_t>>()); // Empty label for learning


	learner.learn(); // Execute learning
	std::cout << learner.toString();


}
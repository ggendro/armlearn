

#include <armlearn/nowaitarmsimulator.h>
#include <armlearn/serialcontroller.h>
#include <armlearn/trajectory.h>

#include <armlearn/setmodepylearner.h>
#include <armlearn/widowxbuilder.h>
#include <armlearn/optimcartesianconverter.h>


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

	armlearn::kinematics::OptimCartesianConverter conv; // Create kinematics calculator
	armlearn::communication::NoWaitArmSimulator arbotix_sim((armlearn::communication::DisplayMode) 0); // Create robot simulator 

	armlearn::WidowXBuilder builder;
	builder.buildConverter(conv);
	builder.buildController(arbotix_sim);

	arbotix_sim.connect();
	arbotix_sim.changeSpeed(50);

	std::cout << "Update servomotors information:" << std::endl;
	arbotix_sim.updateInfos();


    /******************************************************/
    /****        Creation or learning system           ****/
    /******************************************************/

	armlearn::learning::SetModePyLearner learner(&arbotix_sim, &conv, "../../../../files/learnSettings/ddpglearnersettings_ex.json"); // Create learner


    /******************************************************/
    /****           Learning on simulation             ****/
    /******************************************************/

	auto targets = { // Inputs of learning, positions to ask to the robot
		new armlearn::Input<uint16_t>({5, 50, 300})
	};
	for(auto& dest : targets) learner.addToLearningSet(dest, new armlearn::Output<std::vector<uint16_t>>()); // Empty label for learning


	learner.learn(); // Execute learning
	std::cout << learner.toString();


}
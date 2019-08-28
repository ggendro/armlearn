

#include <armlearn/optimcartesianconverter.h>
#include <armlearn/cylindricalconverter.h>


int main(int argc, char *argv[]) {

    /******************************************/
    /****       Converters example         ****/
    /******************************************/

    /*
     * Uses cylindrical and cartesian converters to compute coordinates from servomotors positions, displays results in both coordinate systems.
     * Computes back positions that match the given coordinates, these positions can be different from the initial ones.
     * Finally computes coordinates again to verify that the new positions are correct. Newly computed coordinates should be very close to the one got after the first computation.
     * 
     */

    // Position of first four servomotors
	std::vector<uint16_t> servoPositions = {1500, 2048, 2048, 2048};


	// Servo positions to cylindrical coordinate system computation
	armlearn::kinematics::CylindricalConverter convCyl;
	convCyl.addServo("base", armlearn::kinematics::rotZ, 0, 0, 1, 0, 0, M_PI);
	convCyl.addServo("shoulder", armlearn::kinematics::rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	convCyl.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 5);
	convCyl.addServo("wristAngle", armlearn::kinematics::rotX, 0, 0, 3);


	// Servo positions to cartesian coordinate system computation
	armlearn::kinematics::OptimCartesianConverter convCart;
	convCart.addServo("base", armlearn::kinematics::rotZ, 0, 0, 1, 0, 0, M_PI);
	convCart.addServo("shoulder", armlearn::kinematics::rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
	convCart.addServo("elbow", armlearn::kinematics::rotX, 0, 0, 5);
	convCart.addServo("wristAngle", armlearn::kinematics::rotX, 0, 0, 3);



	std::cout << "Positions of servomotors : ";
	for(auto&& v : servoPositions) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<double> cylindricalCoordinates = convCyl.computeServoToCoord(servoPositions)->getCoord();
	std::cout << "Cylindrical coordinates equivalent : ";
	for(auto&& v : cylindricalCoordinates) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<double> cartesianCoordinates = convCart.computeServoToCoord(servoPositions)->getCoord();
	std::cout << "Cartesian coordinates equivalent : ";
	for(auto&& v : cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<uint16_t> afterCompCyl_servoPositions = convCyl.computeCoordToServo(cylindricalCoordinates)->getServo();
	std::cout << "Positions got after cylindrical computation : ";
	for(auto&& v : afterCompCyl_servoPositions) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<uint16_t> afterCompCart_servoPositions = convCart.computeCoordToServo(cartesianCoordinates)->getServo();
	std::cout << "Positions got after cartesian computation : ";
	for(auto&& v : afterCompCart_servoPositions) std::cout << v << " ";
	std::cout << std::endl;


	std::vector<double> afterCompCyl_cylindricalCoordinates = convCyl.computeServoToCoord(afterCompCyl_servoPositions)->getCoord();
	std::cout << "Coordinates equivalent after cylindrical computation : ";
	for(auto&& v : afterCompCyl_cylindricalCoordinates) std::cout << v << " ";
	std::cout << std::endl;

	std::vector<double> afterCompCart_cartesianCoordinates = convCart.computeServoToCoord(afterCompCart_servoPositions)->getCoord();
	std::cout << "Coordinates equivalent after cartesian computation : ";
	for(auto&& v : afterCompCart_cartesianCoordinates) std::cout << v << " ";
	std::cout << std::endl;
}
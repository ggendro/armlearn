

#include <omp.h>
#include <chrono>

#include "basiccartesianconverter.h"

using namespace armlearn;

int main(int argc, char *argv[]) {


    /******************************************/
    /****   Compute coordinates example    ****/
    /******************************************/

    /*
     * Computes coordinates for all arm positions. Displays the number of invalid positions.
     * 
     * Shape of the array:
     * Test tn (en) : ps1 ps2 ps3   |   x y z   |   cs1 cs2 cs3 |   rx ry rz
     * 
     *  - tn : number of the test display
     *  - en : number of failed tests since now
     *  - ps1 : position of servomotor 1 
     *  - ps2 : position of servomotor 2
     *  - ps3 : position of servomotor 3 
     *  - x : computed x coordinate using forward kinematics
     *  - y : computed y coordinate using forward kinematics
     *  - z : computed z coordinate using forward kinematics
     *  - cs1 : position of servomotor 1 computed using inverse kinematics from coordinates x, y and z
     *  - cs2 : position of servomotor 2 computed using inverse kinematics from coordinates x, y and z
     *  - cs3 : position of servomotor 3 computed using inverse kinematics from coordinates x, y and z
     *  - rx : x coordinate computed again from csx using forward kinematics
     *  - ry : y coordinate computed again from csx using forward kinematics
     *  - rz : z coordinate computed again from csx using forward kinematics
     * 
     * Parallelism does not affetc the results, simply speed up the computations.
     * 
     * Tests show that, for a granularity of 10, the algorithm can compute only 42.24% of positions' corresponding coordinates.
     * 20.42% of coordinates found by the algorithm are not valid.
     * 37.34% of computations fail to finish.
     */

    // Is parallelism enabled
    bool enableParallelism = true;

    // Granularity of coordinates search
    int incr = 100;
    

    int nbTests = 0;
    int nbErr = 0;
    int nbInvalidCoord = 0;
    
    // Variable stating the begin of calculations
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

    #pragma omp parallel for if (enableParallelism)
    for(uint16_t shPos=SHOULDER_MIN; shPos < SHOULDER_MAX; shPos+=incr){

        #pragma omp parallel for if (enableParallelism)
        for(uint16_t elPos=ELBOW_MIN; elPos < ELBOW_MAX; elPos+=incr){

            #pragma omp parallel for if (enableParallelism)
            for(uint16_t wrPos=WRISTANGLE_MIN; wrPos < WRISTANGLE_MAX; wrPos+=incr){

                // Servo positions to cartesian coordinate system computation (created here to avoid segmentation errors due to parallelism)
                
                kinematics::BasicCartesianConverter conv;
                conv.addServo("base", kinematics::fixed, 0, 0, 1, 0, 0, M_PI); // To simplify computation, only the part of the arm moving along the y axis is represented, fixed implies that it does not count as a servomotor
                conv.addServo("shoulder", kinematics::rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
                conv.addServo("elbow", kinematics::rotX, 0, 0, 5);
                conv.addServo("wristAngle", kinematics::rotX, 0, 0, 3);
                // Useless to add wristRotate and gripper

                std::vector<uint16_t> servoPositions = {shPos, elPos, wrPos};

                try{
                    std::vector<double> cartesianCoordinates = conv.computeServoToCoord(servoPositions)->getCoord();

                    std::vector<uint16_t> afterComp_servoPositions = conv.computeCoordToServo(cartesianCoordinates)->getServo();

                    for(auto&& v : afterComp_servoPositions){
                        if(v < BASE_MIN || v > BASE_MAX){
                             #pragma omp atomic
                             nbInvalidCoord++;

                             throw ComputationError("Error : Invalid coordinates");
                             break;
                        }
                    }
                    std::vector<double> afterComp_cartesianCoordinates = conv.computeServoToCoord(afterComp_servoPositions)->getCoord();

                    #pragma omp critical(dataupdate)
                    {
                        std::cout << "Test " << nbTests << " (" << nbErr << " errors) : ";

                        for(auto&& v : servoPositions) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : cartesianCoordinates) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : afterComp_servoPositions) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : afterComp_cartesianCoordinates) std::cout << v << " ";
                        std::cout << std::endl;
                    }

                }catch(ComputationError e){
                    #pragma omp atomic
                    nbErr++;
                    
                    #pragma omp critical(dataupdate)
                    {
                    std::cout << e.what() << std::endl;
                    }
                    
                }

                #pragma omp atomic
                nbTests++;
            }
        }
    }

    // variable stating the end of calculations
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    std::cout << "Execution finished in " << std::chrono::duration<double, std::ratio<1, 1>>(endTime - startTime).count() << " s" << std::endl;

    int succ = nbTests - nbErr;
    std::cout << "End of tests : " << succ << " success over " << nbTests << " tests (" << (((double) succ) / nbTests * 100) << "%)." << std::endl;
    std::cout << "Number of invalid coordinates : " << nbInvalidCoord << " (" << (((double) nbInvalidCoord) / nbTests * 100) << "%)." << std::endl;

    int compErr = nbErr - nbInvalidCoord;
    std::cout << "Number of computation errors : " << compErr << " (" << (((double) compErr) / nbTests * 100) << "%)." << std::endl;


}
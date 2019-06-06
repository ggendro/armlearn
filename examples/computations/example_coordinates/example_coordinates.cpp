

#include <omp.h>
#include <chrono>

#include "cartesianconverter.h"


int main(int argc, char *argv[]) {

    // Is parallelism enabled
    bool enableParallelism = true;

    // Granularity of coordinates search
    int incr = 50;
    

    int nbTests = 0;
    int nbErr = 0;
    int nbInvalidCoord = 0;
    
    // Variable stating the begin of calculations
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

    #pragma omp parallel for if (enableParallelism)
    for(uint16_t shPos=1025; shPos < 3071; shPos+=incr){

        #pragma omp parallel for if (enableParallelism)
        for(uint16_t elPos=1025; elPos < 3071; elPos+=incr){

            #pragma omp parallel for if (enableParallelism)
            for(uint16_t wrPos=1025; wrPos < 3071; wrPos+=incr){

                // Servo positions to cartesian coordinate system computation (created here to avoid segmentation errores du to parallelism)
                CartesianConverter conv;
                //conv.addServo("stand", fixed, 15, 15, 15); // fixed implies that it does not count as a servomotor (in order to simplify computations)
                conv.addServo("base", fixed, 0, 0, 1, 0, 0, M_PI);
                conv.addServo("shoulder", rotX, 0, -2, 5, M_PI/2, 0, M_PI); // Add 90° because of the orientation of the elbow servomotor
                conv.addServo("elbow", rotX, 0, 0, 5);
                conv.addServo("wristAngle", rotX, 0, 0, 3);
                //conv.addServo("wristRotate", rotZ, 0, 0, 2);
                // Useless to add gripper

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
                        std::cout << "Test " << nbTests << "(" << nbErr << " errors) : ";

                        for(auto&& v : servoPositions) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : cartesianCoordinates) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : afterComp_servoPositions) std::cout << v << " ";
                        std::cout << "\t|\t";

                        for(auto&& v : afterComp_cartesianCoordinates) std::cout << v << " ";
                        std::cout << std::endl;
                    }

                    //errSeries = 0;
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
    std::cout << "Execution finished at " << std::chrono::duration<double, std::ratio<1, 1>>(endTime - startTime).count() << " s" << std::endl;

    int succ = nbTests - nbErr;
    std::cout << "End of tests : " << succ << " success over " << nbTests << " tests (" << (((double) succ) / nbTests * 100) << "%)." << std::endl;
    std::cout << "Number of invalid coordinates : " << nbInvalidCoord << " (" << (((double) nbInvalidCoord) / nbTests * 100) << "%)." << std::endl;

    int compErr = nbErr - nbInvalidCoord;
    std::cout << "Number of computation errors : " << compErr << " (" << (((double) compErr) / nbTests * 100) << "%)." << std::endl;


}
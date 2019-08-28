
#include "trajectoryerror.h"

using namespace armlearn;

TrajectoryError::TrajectoryError():TrajectoryError("Error during the execution of a trajectory"){

}

TrajectoryError::TrajectoryError(const std::string& what_arg):std::runtime_error(what_arg){

}

TrajectoryError::TrajectoryError(const char* what_arg):std::runtime_error(what_arg){

}
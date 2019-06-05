
#include "computationerror.h"

ComputationError::ComputationError():ComputationError("Error during calculation"){

}

ComputationError::ComputationError(const std::string& what_arg):std::runtime_error(what_arg){

}

ComputationError::ComputationError(const char* what_arg):std::runtime_error(what_arg){

}
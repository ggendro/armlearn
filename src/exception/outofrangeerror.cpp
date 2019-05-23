
#include "outofrangeerror.h"

OutOfRangeError::OutOfRangeError():OutOfRangeError("Value out of its range"){

}

OutOfRangeError::OutOfRangeError(const std::string& what_arg):std::range_error(what_arg){

}
OutOfRangeError::OutOfRangeError(const char* what_arg):std::range_error(what_arg){

}
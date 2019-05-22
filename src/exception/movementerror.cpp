
#include "movementerror.h"

MovementError::MovementError():MovementError("Device not currently available for movement"){

}

MovementError::MovementError(const std::string& what_arg):std::runtime_error(what_arg){

}
MovementError::MovementError(const char* what_arg):std::runtime_error(what_arg){

}
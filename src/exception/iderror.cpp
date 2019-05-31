
#include "iderror.h"

IdError::IdError():IdError("Invalid ID"){

}

IdError::IdError(const std::string& what_arg):std::runtime_error(what_arg){

}

IdError::IdError(const char* what_arg):std::runtime_error(what_arg){

}
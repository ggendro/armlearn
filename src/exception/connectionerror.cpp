
#include "connectionerror.h"

ConnectionError::ConnectionError():ConnectionError("Unable to establish connection"){

}

ConnectionError::ConnectionError(const std::string& what_arg):std::runtime_error(what_arg){

}

ConnectionError::ConnectionError(const char* what_arg):std::runtime_error(what_arg){

}
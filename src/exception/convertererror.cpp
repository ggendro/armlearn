
#include "convertererror.h"

ConverterError::ConverterError():ConverterError("Error while setting converter"){

}

ConverterError::ConverterError(const std::string& what_arg):std::runtime_error(what_arg){

}

ConverterError::ConverterError(const char* what_arg):std::runtime_error(what_arg){

}
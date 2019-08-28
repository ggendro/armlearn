
#include "typeerror.h"

using namespace armlearn;

TypeError::TypeError():TypeError("Invalid type"){

}

TypeError::TypeError(const std::string& what_arg):std::runtime_error(what_arg){

}

TypeError::TypeError(const char* what_arg):std::runtime_error(what_arg){

}

#include "fileerror.h"

FileError::FileError():FileError("Error while managing file"){

}

FileError::FileError(const std::string& what_arg):std::runtime_error(what_arg){

}

FileError::FileError(const char* what_arg):std::runtime_error(what_arg){

}
#include "VariableException.h"


VariableException::VariableException(int _error_type)
	: error("Error: ") {
	error_type = _error_type;
	error += errors[_error_type];
}


VariableException::~VariableException() {}


const char* VariableException::what() {
	error += "\nLine: " + std::to_string(get_error_line());
	return error.c_str();
}

int VariableException::type() const {
	return error_type;
}
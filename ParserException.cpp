#include "ParserException.h"


ParserException::ParserException(int _error_type) 
	: error("Error: ") {
	error_type = _error_type;
	error += errors[_error_type];
}


ParserException::~ParserException() {}


const char* ParserException::what() {
	error += "\nLine: " + std::to_string(get_error_line());
	return error.c_str();
}

int ParserException::type() const {
	return error_type;
}
#include "exception.h"



Exception::Exception(int _error_type) : error("Error: ") {
	error_type = _error_type;
	error += errors[_error_type];
}

Exception::~Exception() {}


const char* Exception::what() {
	error += "\nLine: " + std::to_string(get_error_line());
	return error.c_str();
}


int Exception::get_error_line() {
	register int line = 1;
	register char* start = prog_start;
	for (line; start != program; start++)
		if (*start == '\r') line++;
	return line;
}

int Exception::type() const {
	return error_type;
}

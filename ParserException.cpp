#include "ParserException.h"


ParserException::ParserException(int _error_type) {
	error_type = _error_type;
}


ParserException::~ParserException() {}


const char* ParserException::what() const {
	return errors[error_type];
}
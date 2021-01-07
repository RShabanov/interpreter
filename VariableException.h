#pragma once
#include <string>
#include "errors.h"


class VariableException {
	int error_type;
	std::string error;
public:
	VariableException(int _error_type);
	~VariableException();

	const char* what();
	int type() const;
};
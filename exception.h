#pragma once
#include <exception>
#include <string>
#include "errors.h"
#include "service_vars.h"


class Exception : public std::exception {
	int error_type;
	std::string error;

	int get_error_line();

public:
	Exception(int _error_type);
	~Exception();

	const char* what();
	int type() const;
};


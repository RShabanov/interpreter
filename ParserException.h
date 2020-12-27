#pragma once


enum error_types {
	INVALID_SYNTAX, NOT_VAR, NOT_FUNC,
	INVALID_FOR, INVALID_WHILE,
	MISS_QUOTE, 
	TOO_MANY_PARAMS, NOT_ENOUGH_PARAMS,
	INVALID_TYPE, UNKNOWN_ERROR,
	EXTRA_BRACKET
};


static const char* errors[]{
	"Invalid syntax.",
	"Not a variable.",
	"Not a function.",
	"Invalid syntax for for-loop.",
	"Invalid syntax for while-loop.",
	"Missing quote.",
	"Too many parameters.",
	"Not enough parameters.",
	"Invalid type.",
	"Unknown error.",
	"Extra bracket."
};


class ParserException {
	int error_type;
public:
	ParserException(int _error_type) { error_type = _error_type; }
	~ParserException();

	const char* what() const { return errors[error_type]; }
};
#pragma once


enum error_types {
	INVALID_SYNTAX,
	NOT_VAR,
	NOT_FUNC,
	INVALID_FOR,
	INVALID_WHILE,
	MISS_QUOTE,
	TOO_MANY_PARAMS,
	NOT_ENOUGH_PARAMS,
	INVALID_TYPE,
	UNKNOWN_ERROR,
	EXTRA_BRACKET,
	UNDEFINED_NAME,
	ALREADY_DEFINED,
	NOT_INITIALIZED,
	NOT_OPEN_FILE,
	BAD_FILE
};


static const char* errors[]{
	"invalid syntax.",
	"not a variable.",
	"not a function.",
	"invalid syntax for for-loop.",
	"invalid syntax for while-loop.",
	"missing quote.",
	"too many parameters.",
	"not enough parameters.",
	"invalid type.",
	"unknown error.",
	"extra bracket.",
	"undefined name.",
	"already defined name.",
	"defining variable without being initialized.",
	"file cannot be opened.",
	"cannot read file properly."
};
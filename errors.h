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
	BRACKET_LACK,
	UNDEFINED_NAME,
	ALREADY_DEFINED,
	NOT_INITIALIZED,
	NOT_OPEN_FILE,
	BAD_FILE,
	ELIF_WITHOUT_IF,
	ELSE_WITHOUT_IF,
	BAD_VALUE,
	INVALID_LOG_BASE,
	STACK_OVERFLOW
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
	"bracket lack.",
	"undefined name.",
	"already defined name.",
	"defining variable without being initialized.",
	"file cannot be opened.",
	"cannot read file properly.",
	"elif cannot be used without \"if\".",
	"else cannot be used without \"if\".",
	"bad value.",
	"invalid logarithm base.",
	"stack overflow."
};
#pragma once


enum token_types {
	NUMBER = 1,
	VARIABLE, 
	STRING, 
	FUNCTION,
	COMMAND, 
	QUOTE, 
	DELIMITER
};

enum tokens {
	PRINT = 1, 
	INPUT,
	IF, 
	ELSE, 
	ELIF,
	WHILE, 
	FOR,
	FUN,
	OPEN_BRACE,
	CLOSE_BRACE,
	LET,
	RETURN, 
	FINISHED, 
	EOL
};


// operators
enum opers {
	EQ = 1, // ==
	NE, // !=
	GE, // >=
	LE, // <=
};

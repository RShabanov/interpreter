#pragma once


enum token_types {
	NUMBER = 1,
	VARIABLE, 
	STRING, 
	FUNCTION,
	COMMAND, 
	QUOTE, 
	DELIMITER,
	COMMENT
};

enum tokens {
	PRINT = 1, 
	INPUT,
	IF, 
	ELSE, 
	ELIF,
	WHILE, 
	FOR,
	BREAK,
	FUN,
	LET,
	AND,
	OR,
	NOT,
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

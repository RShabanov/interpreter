#pragma once


enum token_types {
	NUMBER = 1,
	VARIABLE, 
	STRING, 
	FUNCTION,
	COMMAND, 
	QUOTE, 
	DELIMITER,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
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
	LET,
	COLON,
	END, 
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
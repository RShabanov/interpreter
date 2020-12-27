#pragma once
#include "ParserException.h"
#include <cstring>
#include <string>


enum token_types {
	NUMBER = 1,
	VARIABLE, STRING, FUNCTION,
	COMMAND, QUOTE, DELIMITER,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
};

enum tokens {
	PRINT = 1, INPUT,
	IF, ELSE, ELIF,
	WHILE, FOR,
	DEF, COLON,
	END, RETURN, FINISHED, EOL
};

// operators
enum opers {
	EQ = 1, // ==
	NE, // !=
	GE, // >=
	LE, // <=
};

struct Commands {
	const char* command;
	int tok;
} cmd_table[];


class Parser {
	//char* token;
	std::string token;
	int tok;
	int token_type;
	char* program;

	bool is_eof() const;
	bool is_space() const;
	bool is_cr() const; // carriage return
	bool is_opers() const;
	bool is_delim() const;
	bool is_quote() const;

	void token_eof();
	void token_cr();
	void token_opers(char* _temp);
	void token_delim(char* _temp);
	void token_quote(char* _temp);
	void token_number(char* _temp);
	void token_string(char* _temp);

	void parse_brackets(double& result); // ������� ������ ()
	void parse_unary(double& result); // ������� ������� ��������
	void parse_power(double& result); // ������� �������
	void parse_mul_div(double& result); // ������� ���������\�������
	void parse_add_sub(double& result); // ������� ��������\���������
	void parse_relation_oper(double& result); // ������� ����������� ���������

	void read_token();

	int find_cmd() const;

public:
	Parser(char* _prog);
	~Parser();

	void parse(double& result);
};
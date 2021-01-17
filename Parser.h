#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include "token.h"
#include "brackets.h"
#include "variables.h"


class Parser {

	bool is_cr(char) const; // carriage return
	bool is_eof(char) const;
	bool is_space(char) const;
	bool is_opers(char) const;
	bool is_delim(char) const;
	bool is_quote(char) const;
	bool is_number(char*) const;
	bool is_string(char) const;
	bool is_comment(char) const;
	bool is_escape_char(char*) const;

	void token_eof();
	void token_cr();
	void token_opers();
	void token_delim();
	void token_quote();
	void token_number();
	void token_string();
	void token_comment();

	void parse_brackets(double& result); // парсинг скобок ()
	void parse_unary(double& result); // парсинг унарных операций
	void parse_power(double& result); // парсинг степени
	void parse_mul_div(double& result); // парсинг умножения\деления
	void parse_add_sub(double& result); // парсинг сложения\вычитания
	void parse_relation_oper(double& result); // парсинг реляционных выражений

	void read_values(double& result);
	void skip_space();

	char get_escape_char(char* _str) const;

public:
	Parser();
	~Parser();
	
	bool is_end() const;
	bool is_eof() const;
	bool is_eol() const;
	bool is_expression(char);

	void read_token();
	void putback_token();
	void parse(double& result);
};

extern Parser parser;
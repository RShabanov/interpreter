#pragma once
#include <iostream>
#include <fstream>
#include <string>
//#include "Parser.h"
#include "commands.h"


class Scanner
{
	Scanner(const Scanner&);

	void reset(std::string& _str) const;

	void print_tabs() const;

	void token_variable() const;
	void token_expression() const;

public:
	Scanner();
	~Scanner();

	void run();
	void print_help() const;
};


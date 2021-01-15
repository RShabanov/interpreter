#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
//#include "Parser.h"
#include "commands.h"


class Scanner
{
	Scanner(const Scanner&);

	void reset(const char* _str) const;

	void print_tabs() const;

	void token_variable() const;
	void token_expression() const;

	void read_file(const char* filename, std::string&) const;

public:
	Scanner();
	~Scanner();

	void run();
	void run(const char* filename);
	void print_help() const;
};


#pragma once
#include <iostream>
#include <fstream>
#include <string>
//#include "Parser.h"
#include "commands.h"


class Init
{
	Init(const Init&);

	void reset(std::string& _str) const;

	void token_variable() const;
	void token_expression() const;

	void define_variable() const;

public:
	Init();
	~Init();

	void run();
	void print_help() const;
};


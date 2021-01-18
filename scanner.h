#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
#include "commands.h"


class Scanner
{
	Scanner(const Scanner&);

	void read_file(const char* filename, std::string&) const;
	void reset(const char* _str) const;
public:
	Scanner();
	~Scanner();

	void run(const char* filename);
};


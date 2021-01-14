#pragma once
#include <istream>
#include <string>


extern std::istream* in;

extern char* program, *prog_start;

extern std::string token;
extern int token_type; // содержит тип элемента
extern int tok; // содержит внутреннее представление элемента 
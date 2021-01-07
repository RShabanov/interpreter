#pragma once
#include <string>
#include <vector>


extern std::vector<char> parentheses;

extern char* program, *prog_start;

extern std::string token;
extern int token_type; // содержит тип элемента
extern size_t tok; // содержит внутреннее представление элемента 
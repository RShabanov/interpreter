#pragma once
#include <cstring>
#include <vector>
#include "exception.h"


// stack for brackets like () and {}
class Brackets {
	int braces_amt; // for {}
	std::vector<char> brs;

	bool is_open_bracket(char) const;
	bool is_diff_back(char) const;
public:
	Brackets();
	~Brackets();

	size_t braces_size() const;
	size_t size() const;
	char back() const;

	void clear();
	void push(char);
	void putback(char);

	bool is_brace(char) const;
	bool is_bracket(char) const;
	bool is_parenthesis(char) const;
	bool empty() const;
};

extern Brackets brackets;
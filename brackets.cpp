#include "brackets.h"


Brackets brackets;


Brackets::Brackets() {
	braces_amt = 0;
}

Brackets::~Brackets() {}


void Brackets::push(char br) {
	if (is_open_bracket(br)) {
		if (br == '{') braces_amt++;
		brs.push_back(br);
	}
	else {
		if (brs.empty()) throw Exception(EXTRA_BRACKET);
		if (is_diff_back(br)) throw Exception(INVALID_SYNTAX);

		if (br == '}') braces_amt--;
		brs.pop_back();
	}
}

void Brackets::putback(char br) {
	if (is_open_bracket(br)) {
		if (brs.empty() || br != brs.back())
			throw Exception(UNKNOWN_ERROR);
		else brs.pop_back();
	}
	else if (br == ')') brs.push_back('(');
	else if (br == '}') {
		brs.push_back('{');
		braces_amt++;
	}
}

void Brackets::clear() {
	braces_amt = 0;
	brs.clear();
}


size_t Brackets::braces_size() const {
	return braces_amt;
}

size_t Brackets::parenthesis_size() const {
	return brs.size() - braces_amt;
}

size_t Brackets::size() const {
	return brs.size();
}

char Brackets::back() const {
	return brs.back();
}


bool Brackets::empty() const {
	return brs.empty();
}

bool Brackets::is_parenthesis(char symbol) const {
	return strchr("()", symbol);
}

bool Brackets::is_brace(char symbol) const {
	return strchr("{}", symbol);
}

bool Brackets::is_bracket(char symbol) const {
	return is_brace(symbol) || is_parenthesis(symbol);
}

inline bool Brackets::is_open_bracket(char br) const {
	return br == '(' || br == '{';
}

inline bool Brackets::is_diff_back(char br) const {
	return br == ')' && brs.back() == '{' ||
		br == '}' && brs.back() == '(';
}
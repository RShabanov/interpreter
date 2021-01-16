#include "Function.h"


Function fun;

Function::Function() : funs() {}

Function::~Function() {}


bool Function::is_fun(const std::string& key) {
	// проверяет, есть ли такая функция
	// с таким же именем и числом параметров

	auto range = funs.equal_range(key);

	if (range.first != range.second) {
		int argc = check_params();
		auto it = range.first;
		for (it; it != range.second; it++)
			if (it->second.argc == argc) {
				token = it->first;
				token_type = FUNCTION;
				return true;
			}
		it--;

		throw Exception(it->second.argc > argc ?
			NOT_ENOUGH_PARAMS : TOO_MANY_PARAMS);
	}
	return false;
}

int  Function::check_params() {
	register char* temp = program;
	parser.read_token();
	if (token[0] == '(') {
		int cnt = 0;
		parser.read_token();
		if (token[0] != ')') {
			parser.putback_token();
			cnt++;
			do {
				parser.read_token();
				if (token[0] == ',') cnt++;
			} while (token[0] != ')');
		}
		program = temp;
		return cnt;
	}
	else throw Exception(INVALID_SYNTAX);
}


void Function::add_fun(const std::string& _name,
	std::string& _body, int _argc) {
	funs.insert({ _name, Fun(_body, _argc) });
}

void Function::delete_fun(const std::string& key) {
	auto it = --funs.equal_range(key).second;
	if (it != funs.end())
		funs.erase(it);
	else throw Exception(UNKNOWN_ERROR);
}
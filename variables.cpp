#include "variables.h"


Var var;

Var::Var() {}

Var::~Var() {}


bool Var::is_var(const std::string& key) {
	for (auto it = vars.rbegin(); it != vars.rend(); it++)
		if (it->find(key) != it->end())
			return true;
	return false;
}

bool Var::in_last_namespace(const std::string& key) {
	if (!vars.empty()) {
		auto it = --vars.end();
		return it->find(key) != it->end();
	}
	else throw Exception(UNKNOWN_ERROR);
}

double Var::get_var(const std::string& key) {
	for (auto it = vars.rbegin(); it != vars.rend(); it++) {
		auto var = it->find(key);
		if (var != it->end())
			return var->second;
	}
	throw Exception(UNDEFINED_NAME);
}

void Var::assign_var(const std::string& key, double value) {
	for (auto it = vars.rbegin(); it != vars.rend(); it++) {
		auto var = it->find(key);
		if (var != it->end()) {
			var->second = value;
			return;
		}
	}
	throw Exception(UNDEFINED_NAME);
}

void Var::create_var(const std::string& key, double value) {
	if (vars.empty())
		(vars.begin())->insert({ key, value });
	else (--vars.end())->insert({ key, value });
}

void Var::delete_var(const std::string & key) {
	for (auto it = vars.rbegin(); it != vars.rend(); it++) {
		auto var = it->find(key);
		if (var != it->end()) {
			it->erase(var);
			return;
		}
	}
	throw Exception(UNKNOWN_ERROR);
}

void Var::create_namespace() {
	if (vars.max_size() != vars.size()) {
		std::map<std::string, double> ns;
		vars.push_back(ns);
	}
	else throw Exception(STACK_OVERFLOW);
}

void Var::delete_last_namespace() {
	if (!vars.empty()) vars.pop_back();
	else throw Exception(UNKNOWN_ERROR);
}

void Var::clear() { vars.clear(); }
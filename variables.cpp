#include "variables.h"


Var var;

Var::Var() {}
Var::~Var() {}

bool Var::is_var(const std::string& key) {
	return vars.find(key) != vars.end();
}

double Var::get_var(const std::string& key) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		return it->second;
	else throw Exception(UNDEFINED_NAME);
}

void Var::assign_var(const std::string& key, double value) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		it->second = value;
	else throw Exception(UNDEFINED_NAME);
}

void Var::create_var(const std::string& key, double value) {
	vars.insert({ key, value });
}

void Var::delete_var(const std::string& key) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		vars.erase(it);
	else throw Exception(UNKNOWN_ERROR);
}
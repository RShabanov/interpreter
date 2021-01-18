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


void Var::restore_with_changes(std::multimap<std::string, double>& _vars) {
	std::set<std::string> var_name;
	for (auto it : _vars) var_name.insert(it.first);

	for (auto name : var_name) {
		auto range = vars.equal_range(name);
		auto vars_range = _vars.equal_range(name);

		for (auto i = range.first, vi = vars_range.first;
			vi != vars_range.second && i != range.second;
			i++, vi++) {
			vi->second = i->second;
		}
	}
	vars = _vars;
}


void Var::restore(std::multimap<std::string, double>& _vars) {
	vars = _vars;
}

void Var::copy_to(std::multimap<std::string, double>& target) {
	target = vars;
}
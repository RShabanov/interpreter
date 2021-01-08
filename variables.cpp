#include "variables.h"


Vars vars;
Funs funs;


bool is_var(const Name& key) {
	return vars.find(key) != vars.end();
}

double get_var(const Name& key) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		return it->second;
	else throw VariableException(UNDEFINED_NAME);
}

void assign_var(const Name& key, double value) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		it->second = value;
	else throw VariableException(UNDEFINED_NAME);
}

void create_var(const Name& key, double value) {
	vars.insert(Variable(key, value));
}

void delete_var(const Name& key) {
	auto it = --vars.equal_range(key).second;
	if (it != vars.end())
		vars.erase(it);
	else throw VariableException(UNKNOWN_ERROR);
}



bool is_function(const Name& key) {
	return funs.find(key) != funs.end();
}
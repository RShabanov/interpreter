#include "variables.h"


std::vector<Variable> variables;
std::vector<Function> functions;



bool is_variable(const char* _var, size_t& pos) {
	pos = variables.size();
	while (pos > 0)
		if (!strcmp(&variables[--pos].name[0], _var))
			return true;
	return false;
}

bool is_function(const char* _fn, size_t& pos) {
	pos = functions.size();
	while (pos > 0)
		if (!strcmp(&functions[--pos].name[0], _fn))
			return true;
	return false;
}



double get_variable(size_t index) {
	if (index < variables.size())
		return variables[index].value;
	else throw VariableException(UNDEFINED_NAME);
}

const char* get_function(size_t index) {
	if (index < functions.size())
		return &functions[index].prog[0];
	else throw VariableException(UNDEFINED_NAME);
}


void create_variable(const char* _var, double _var_value) {
	Variable* ptr_temp = new Variable(_var, _var_value);
	variables.push_back(*ptr_temp);
	delete ptr_temp;
}


void change_var_value(const size_t& index, const double& _value) {
	if (index < variables.size())
		variables[index].value = _value;
	else throw VariableException(UNKNOWN_ERROR);
	// на самом деле ошибка понятна,
	// но что вывести пользователю непонятно,
	// поэтому будет выводиться ошибка такого типа
}


void delete_last_var() {
	if (!variables.empty())
		variables.pop_back();
	else throw VariableException(UNKNOWN_ERROR);
}
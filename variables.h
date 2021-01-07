#pragma once
#include <vector>
#include <string>
#include <cstring>
#include "VariableException.h"


struct Variable {
	std::string name;
	double value;
	Variable(std::string _name, double _value)
		: name(_name), value(_value) {}
};

struct Function {
	std::string name, prog;
};


void create_variable(const char* _var, double _var_value);
void change_var_value(const size_t& index, const double& _value);
void delete_last_var();

void create_function();


bool is_variable(const char* _var, size_t& pos);
bool is_function(const char* _func, size_t& pos);


//double get_variable(const char* _var);
double get_variable(size_t index);
const char* get_function(int);
#pragma once
#include <string>
#include <map>
#include "exception.h"



class Var {
	std::multimap<std::string, double> vars;

	friend class Cmd;
public:
	Var();
	~Var();

	bool is_var(const std::string&);
	double get_var(const std::string&);
	void delete_var(const std::string&);
	void assign_var(const std::string&, double);
	void create_var(const std::string&, double = NAN);
};

extern Var var;
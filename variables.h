#pragma once
#include <string>
#include <map>
#include <set>
#include "exception.h"



class Var {
	std::multimap<std::string, double> vars;
public:
	Var();
	~Var();

	bool is_var(const std::string&);
	double get_var(const std::string&);
	void delete_var(const std::string&);
	void assign_var(const std::string&, double);
	void create_var(const std::string&, double = NAN);

	// leaves variables which have the same names with new values
	void restore_with_changes(std::multimap<std::string, double>& _vars);
	
	// set vars equal to _vars
	void restore(std::multimap<std::string, double>& _vars);
	void copy_to(std::multimap<std::string, double>& target);

	void clear();
};

extern Var var;
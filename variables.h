#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include "exception.h"



class Var {
	std::vector<std::map<std::string, double>> vars;
public:
	Var();
	~Var();

	bool is_var(const std::string&);
	bool in_last_namespace(const std::string&);
	double get_var(const std::string&);
	void delete_var(const std::string&);
	void assign_var(const std::string&, double);
	void create_var(const std::string&, double = NAN);

	void create_namespace();
	void delete_last_namespace();

	void clear();
};

extern Var var;
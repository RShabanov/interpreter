#pragma once
#include <string>
#include <cstring>
#include <map>
#include "VariableException.h"


typedef std::string Name;
typedef std::pair<Name, double> Variable;
typedef std::multimap<Name, std::string> Funs;
typedef std::multimap<Name, double> Vars;

extern Vars vars;
extern Funs funs;


bool is_var(const Name&);
double get_var(const Name&);
void delete_var(const Name&);
void assign_var(const Name&, double);
void create_var(const Name&, double = NAN);



bool is_function(const Name&);
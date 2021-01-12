#pragma once
#include <string>
#include <map>
#include "exception.h"


bool is_var(const std::string&);
double get_var(const std::string&);
void delete_var(const std::string&);
void assign_var(const std::string&, double);
void create_var(const std::string&, double = NAN);
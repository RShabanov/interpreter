#pragma once
#include <cstring>
#include <map>
#include <set>
#include "token.h"
#include "Parser.h"
#include "Function.h"



static class Cmd {
	std::set<int> return_cmd;
	std::map<std::string, int> cmd_table;

	void define_variable();
	void out_string(std::string&);

	int get_params_cnt();
	void get_fun_body(std::string&);

	void skip_if();
	void skip_rest_conditional();
	void following_branch();

	void cmd_print();
	void cmd_input();
	void cmd_if();
	void cmd_else();
	void cmd_elif();
	void cmd_while();
	void cmd_for();
	void cmd_fun();
	void cmd_let();
	void cmd_return();
public:
	Cmd();
	~Cmd();

	bool is_return_cmd(int);
	bool is_cmd(const std::string& _cmd, int& pos);
	
	void execute(int cmd_token);
} cmd;



class Executive {
	void read_exp(std::string&) const;
	void eval_var() const;
	void invert_opers() const; // opers from token.h

	bool not_executive() const;

	double get_value() const;

public:
	void eval(const char*);
	void assign_variable() const;
	double compute_exp() const;
};
extern Executive exec;



static class FunFunctor {
	void read_param_value(std::vector<double>&);
	void execute(std::vector<double>&);
	void add_fun_vars(std::vector<double>&);
	//void del_fun_vars(std::multimap<std::string, double>&);
public:
	void operator()(const std::string&);
} execute_fun;
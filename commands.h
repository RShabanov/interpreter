#pragma once
#include <cstring>
#include <map>
#include <set>
#include "token.h"
#include "Parser.h"



class Cmd {
	std::set<int> return_cmd;
	std::map<std::string, int> cmd_table;

	void define_variable();
	void out_string(std::string&);

	int get_params_cnt();
	void get_fun_body(std::string&);

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
};
extern Cmd cmd;


class Function {

	// fun function_name(let var1, var2, ...) {
	//		... function_body
	//		[return]
	// }

	struct Fun {
		std::string body;
		int argc = 0;
		Fun(std::string& _body, int _argc) 
			: body(std::move(_body)) {
			argc = _argc;
		}
	};

	std::multimap<std::string, Fun> funs;

	int check_params();
	void read_param_value(std::vector<double>&);
	void exec_fun(std::vector<double>&);
	void add_fun_vars(std::vector<double>&);
	void del_fun_vars(std::multimap<std::string, double>&);

	friend class Cmd;
public:
	Function();
	~Function();

	bool is_fun(const std::string&);

	void add_fun(const std::string& _name, 
		std::string& _body, int _argc);
	void delete_fun(const std::string&);
	void execute(const std::string&);
};
extern Function fun;



class Executive {
	void read_exp(std::string&) const;
	void eval_var() const;

	bool not_executive() const;

	double get_value() const;
public:
	void eval(const char*);
	void assign_variable() const;
	double compute_exp() const;

};
extern Executive exec;
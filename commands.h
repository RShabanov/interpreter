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

	void del_vars(std::multimap<std::string, double>&);

	void cmd_print();
	void cmd_input();
	void cmd_if();
	void cmd_else();
	void cmd_elif();
	void cmd_while();
	void cmd_for();
	void cmd_fun();
	void cmd_open_brace();
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
		int args = 0;
		bool return_type = false;
	};

	std::multimap<std::string, Fun> funs;

	friend class Cmd;
public:
	Function();
	~Function();

	bool is_fun(const std::string&) const;
	bool is_return_fun(const std::string&) const;

	void add_fun(const char*);
	void delete_fun(std::string&);
	void execute(std::string&) const;
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
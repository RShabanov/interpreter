#pragma once
#include <cstring>
#include <map>
#include <set>
#include "token.h"
#include "Parser.h"



void assign_variable();
double compute_exp();


class Cmd {
	std::set<int> return_cmd;
	std::map<std::string, int> cmd_table;

	void define_variable();
	void out_string(std::string&);

	void cmd_print();
	void cmd_input();
	void cmd_if();
	void cmd_else();
	void cmd_elif();
	void cmd_while();
	void cmd_for();
	void cmd_fun();
	void cmd_open_brace();
	void cmd_close_brace();
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
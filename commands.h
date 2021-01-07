#pragma once
#include <cstring>
#include "token.h"
#include "Parser.h"


extern int cmd_table_size;

extern struct CommandsTable {
	const char* command;
	int token;
} cmd_table[];

// ------------------------

class Cmd {

	void assign_var();
	void define_var(double& _value, bool& is_defined);

	void cmd_print();
	void cmd_input();
	void cmd_if();
	void cmd_else();
	void cmd_elif();
	void cmd_while();
	void cmd_for();
	void cmd_def();
	void cmd_let();
	void cmd_return();

public:
	Cmd();
	~Cmd();

	bool is_cmd(const char* _cmd, size_t& pos);
	
	void execute(int cmd_token);
};

extern Cmd cmd;

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

void assign_variable();


class Cmd {

	void define_variable();

	void out_string(std::string&);

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

	bool is_cmd(const char* _cmd, int& pos);
	
	void execute(int cmd_token);
};

extern Cmd cmd;

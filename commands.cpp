#include "commands.h"


int cmd_table_size = 11;

CommandsTable cmd_table[] = {
	"print", PRINT,
	"input", INPUT,
	"if", IF,
	"else", ELSE,
	"elif", ELIF,
	"while", WHILE,
	"for", FOR,
	"fun", FUN,
	"let", LET,
	"return", RETURN,
	//":", COLON,
	"", END
};

Cmd cmd;


// ----------------------------------

Cmd::Cmd() {}
Cmd::~Cmd() {}


void Cmd::execute(int cmd_token) {
	switch (cmd_token) {
	case PRINT:
		cmd_print();
		break;
	case INPUT:
		cmd_input();
		break;
	case IF:
		cmd_if();
		break;
	case ELSE:
		cmd_else();
		break;
	case ELIF:
		cmd_elif();
		break;
	case WHILE:
		cmd_while();
		break;
	case FOR:
		cmd_for();
		break;
	case FUN:
		cmd_def();
		break;
	case LET:
		cmd_let();
		break;
	case RETURN:
		cmd_return();
		break;
	}
}


void Cmd::cmd_print() {
	parser.read_token();
	if (token[0] == '(') {
		std::string result_str;
		out_string(result_str); // подготовить строку к выводу

		if (parentheses.empty())
			std::cout << result_str << std::endl;
		else throw ParserException(EXTRA_BRACKET);
	}
	else throw ParserException(INVALID_SYNTAX);
}

void Cmd::cmd_input() {

}

void Cmd::cmd_if() {}
void Cmd::cmd_else() {}
void Cmd::cmd_elif() {}
void Cmd::cmd_while() {}
void Cmd::cmd_for() {}
void Cmd::cmd_def() {}
void Cmd::cmd_let() {
	// на вход program указывает на "var = 5"

	register size_t cnt = 0;

	try {
		do {
			parser.read_token();
			if (token_type == STRING) assign_var();
			else if (token_type == VARIABLE)
				throw VariableException(ALREADY_DEFINED);
			else throw ParserException(INVALID_SYNTAX);

			cnt++;

		} while (tok != EOL && tok != FINISHED);
	}
	catch (ParserException& e) {
		while (cnt-- > 0) delete_last_var();
		throw ParserException(e.type());
	}
	catch (VariableException& e) {
		while (cnt-- > 0) delete_last_var();
		throw VariableException(e.type());
	}

}

void Cmd::cmd_return() {}



////////////////////////////////////////
////////////////////////////////////////
//
// AUXILIARY METHODS
//

bool Cmd::is_cmd(const char* _cmd, size_t& pos) {
	for (register int i = 0; *cmd_table[i].command; i++)
		if (!strcmp(cmd_table[i].command, _cmd)) {
			pos = cmd_table[i].token;
			return true;
		}
	pos = -1;
	return false;
}


void Cmd::assign_var() {
	register bool is_defined = false;
	std::string var_name(token);

	parser.read_token();
	if (token_type == DELIMITER) {
		double var_value;
		define_var(var_value, is_defined);

		if (is_defined) create_variable(var_name.c_str(), var_value);
		else throw VariableException(NOT_INITIALIZED);
	}
	else throw ParserException(INVALID_SYNTAX);
}


void Cmd::define_var(double& _value, bool& is_defined) {
	if (tok != EOL && tok != FINISHED) {
		if (token[0] == '=') {
			parser.parse(_value);
			parser.read_token();

			is_defined = true;
		}
		else throw ParserException(INVALID_SYNTAX);
	}
}



void Cmd::out_string(std::string& result_str) {
	register double result;

	while (true) {
		parser.read_token();
		if (tok == EOL || tok == FINISHED)
			break; // если отсутсвует ) продолжить ввод

		switch (token_type) {
		case QUOTE:
			result_str += token;
			break;
		default:
			parser.putback_token();
			parser.parse(result);

			result_str += std::to_string(result);
			break;
		}
		parser.read_token();

		if (token[0] == ',')
			result_str += " ";
	}
}
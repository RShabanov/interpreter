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


void assign_variable() {
	// input: string like "a = b = 5" or "a = 5", etc.

	register char* start = const_cast<char*>(program);
	register double res;

	parser.read_token();
	register Name target(token);

	parser.read_token();
	if (token[0] == ',') return;
	if (token[0] == '=') {
		parser.read_token();
		parser.putback_token();

		if (token_type == VARIABLE) {
			register Name source(token);

			assign_variable();
			if (*program) parser.parse(res);
			else res = get_var(source);
		}
		else {
			parser.parse(res);
			if (!is_end() && token[0] != ',') 
				throw ParserException(INVALID_SYNTAX);
		}

		assign_var(target, res);
	}
	else {
		parser.putback_token();
		program = start;
		parser.parse(res);

		if (!is_end() && token[0] != ',') 
			throw ParserException(INVALID_SYNTAX);
		program = start;
	}
}


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
	std::vector<Name> var_map;

	try {
		do {
			parser.read_token();

			if (token_type == STRING) {
				define_variable();
				var_map.push_back(token);
			}
			else if (token_type == VARIABLE)
				throw VariableException(ALREADY_DEFINED);
			else throw ParserException(INVALID_SYNTAX);

		} while (!is_end());
	}
	catch (ParserException& e) {
		for (auto var_name : var_map) delete_var(var_name);
		throw ParserException(e.type());
	}
	catch (VariableException& e) {
		for (auto var_name : var_map) delete_var(var_name);
		throw VariableException(e.type());
	}

}

void Cmd::cmd_return() {}



////////////////////////////////////////
////////////////////////////////////////
//
// AUXILIARY METHODS
//

bool Cmd::is_cmd(const char* _cmd, int& pos) {
	for (register int i = 0; *cmd_table[i].command; i++)
		if (!strcmp(cmd_table[i].command, _cmd)) {
			pos = cmd_table[i].token;
			return true;
		}
	pos = -1;
	return false;
}


void Cmd::define_variable() {
	register char* line = program - token.length();
	Name var_name(token);

	parser.read_token();
	if (token[0] == ',' || is_end()) {
		create_var(var_name);
		return;
	}
	else if (token[0] == '=') {
		create_var(var_name);

		parser.putback_token();
		program = line;

		assign_variable();
		parser.read_token();
	}
	else throw ParserException(INVALID_SYNTAX);
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
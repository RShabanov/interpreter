#include "commands.h"


const char return_cmd[] = { INPUT, RETURN };

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

		if (token_type == VARIABLE) {
			register char* temp = program - token.length();
			register Name source(token);

			parser.read_token();
			if (is_end()) res = get_var(source);
			else {
				parser.putback_token();
				program = temp;
				if (token[0] == '=') {
					assign_variable();
					res = get_var(source);
				}
				else {
					res = compute_exp();
					parser.read_token();
					if (!is_end()) throw ParserException(INVALID_SYNTAX);
				}
			}
		}
		else {
			parser.putback_token();
			res = compute_exp();
			parser.read_token();

			if (!is_end() && token[0] != ',')
				throw ParserException(INVALID_SYNTAX);
		}
		assign_var(target, res);
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
	// ограничимся пока вводом double
	register char* start = program;

	parser.read_token();
	if (token[0] == '(') {
		register std::string invitational_str;
		parser.read_token();

		if (token_type == QUOTE) invitational_str += token;
		else {
			parser.putback_token();
			if (token[0] != ')')
				invitational_str += std::to_string(compute_exp());
		}

		parser.read_token();
		if (token[0] == ')') {
			std::cout << invitational_str;
			std::getline(std::cin, token);
		}
		else if (is_end() && !parentheses.empty())
			throw ParserException(EXTRA_BRACKET);
		else throw ParserException(INVALID_SYNTAX);
	}
	else throw ParserException(INVALID_SYNTAX);
}


void Cmd::cmd_let() {
	// на вход program указывает на "var = 5"
	std::vector<Name> var_map;

	try {
		do {
			parser.read_token();

			if (token_type == STRING) {
				var_map.push_back(token);
				define_variable();
			}
			else if (token_type == VARIABLE)
				throw VariableException(ALREADY_DEFINED);
			else throw ParserException(INVALID_SYNTAX);

		} while (!is_end());
	}
	catch (ParserException& e) {
		for (auto var_name : var_map) 
			if (is_var(var_name)) delete_var(var_name);
		throw e;
	}
	catch (VariableException& e) {
		for (auto var_name : var_map)
			if (is_var(var_name)) delete_var(var_name);
		throw e;
	}

}


void Cmd::cmd_if() {}
void Cmd::cmd_else() {}
void Cmd::cmd_elif() {}
void Cmd::cmd_while() {}
void Cmd::cmd_for() {}
void Cmd::cmd_def() {}
void Cmd::cmd_return() {}

// sin
// cos
// log
// ect



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

bool Cmd::is_return_cmd(int cmd_tok) {
	for (int c_tok : return_cmd)
		if (c_tok == cmd_tok) return true;
	return false;
}


void Cmd::define_variable() {
	register char* line = program - token.length();
	Name var_name(token);
	create_var(var_name);
	
	parser.read_token();
	if (token[0] == ',' || is_end()) return;
	else if (token[0] == '=') {
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
		if (is_end())
			break; // если отсутсвует ) продолжить ввод

		switch (token_type) {
		case QUOTE:
			result_str += token;
			break;
		case FUNCTION:
			break;
		default:
			if (is_cmd(token.c_str(), tok) && 
				is_return_cmd(tok)) {
				cmd_input();
				result_str += token;
			}
			else {
				parser.putback_token();
				parser.parse(result);

				result_str += std::to_string(result);
			}
			break;
		}
		parser.read_token();

		if (token[0] == ',')
			result_str += " ";
	}
}


double compute_exp() {
	// for things like:
	// 2 * input() ...
	// input() * input() - 5 * input(), etc

	std::string exp;

	while(true) {
		parser.read_token();

		if (token_type == VARIABLE || 
			token_type == NUMBER ||
			parser.is_expression(token[0])) exp += token;
		else if (cmd.is_cmd(token.c_str(), tok)) {
			if (cmd.is_return_cmd(tok)) {
				cmd.execute(tok);
				exp += token;
			}
			else throw ParserException(INVALID_TYPE);
		}
		else if (token_type == FUNCTION) {

		}
		else if (token_type == STRING) // чтобы нормально причину ошибки выдавала
			throw VariableException(UNDEFINED_NAME);
		else break;
	}
	parser.putback_token();

	register double res;
	register char* temp = program;

	try {
		program = const_cast<char*>(exp.c_str());
		parser.parse(res);
	}
	catch (ParserException& e) {
		program = temp;
		throw e;
	}
	catch (VariableException& e) {
		program = temp;
		throw e;
	}
	program = temp;
	return res;
}
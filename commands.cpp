#include "commands.h"



Cmd cmd;


void assign_variable() {
	// input: string like "a = b = 5" or "a = 5", etc.

	register char* start = const_cast<char*>(program);
	register double res;

	parser.read_token();
	register std::string target(token);

	parser.read_token();
	if (token[0] == ',') return;
	if (token[0] == '=') {
		parser.read_token();

		if (token_type == VARIABLE) {
			register char* temp = program - token.length();
			register std::string source(token);

			parser.read_token();
			if (parser.is_end()) res = get_var(source);
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
					if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
				}
			}
		}
		else {
			parser.putback_token();
			res = compute_exp();
			parser.read_token();

			if (!parser.is_end() && token[0] != ',')
				throw Exception(INVALID_SYNTAX);
		}
		assign_var(target, res);
	}
}

double compute_exp() {
	// for things like:
	// 2 * input() ...
	// input() * input() - 5 * input(), etc

	std::string exp;

	while (true) {
		parser.read_token();

		if (token_type == VARIABLE ||
			token_type == NUMBER ||
			parser.is_expression(token[0])) exp += token;
		else if (cmd.is_cmd(token, tok)) {
			if (cmd.is_return_cmd(tok)) {
				cmd.execute(tok);
				exp += token;
			}
			else throw Exception(INVALID_TYPE);
		}
		else if (token_type == FUNCTION) {

		}
		else if (token_type == STRING) // чтобы нормально причину ошибки выдавала
			throw Exception(UNDEFINED_NAME);
		else break;
		
		exp += " ";
	}
	parser.putback_token();

	register double res;
	register char* temp = program;

	try {
		program = const_cast<char*>(exp.c_str());
		parser.parse(res);
	}
	catch (Exception& e) {
		program = temp;
		throw e;
	}

	program = temp;
	return res;
}


// ----------------------------------

Cmd::Cmd() : return_cmd{ INPUT, RETURN },
	cmd_table{
			{"print", PRINT},
			{"input", INPUT},
			{"if", IF},
			{"else", ELSE},
			{"elif", ELIF},
			{"while", WHILE},
			{"for", FOR},
			{"fun", FUN},
			{"{", OPEN_BRACE},
			{"}", CLOSE_BRACE},
			{"let", LET},
			{"return", RETURN},
	} {}

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
		cmd_fun();
		break;
	case OPEN_BRACE:
		cmd_open_brace();
		break;
	case CLOSE_BRACE:
		cmd_close_brace();
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

		if (brackets.empty())
			std::cout << result_str << std::endl;
		else throw Exception(EXTRA_BRACKET);
	}
	else throw Exception(INVALID_SYNTAX);
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
		else if (parser.is_end() && !brackets.empty())
			throw Exception(EXTRA_BRACKET);
		else throw Exception(INVALID_SYNTAX);
	}
	else throw Exception(INVALID_SYNTAX);
}


void Cmd::cmd_let() {
	// на вход program указывает на "var = 5"
	std::vector<std::string> var_map;

	try {
		do {
			parser.read_token();

			if (token_type == STRING && !is_cmd(token, tok)) {
				var_map.push_back(token);
				define_variable();
			}
			else if (token_type == VARIABLE)
				throw Exception(ALREADY_DEFINED);
			else throw Exception(INVALID_SYNTAX);

		} while (!parser.is_end());
	}
	catch (Exception& e) {
		for (auto var_name : var_map) 
			if (is_var(var_name)) delete_var(var_name);
		throw e;
	}
}


void Cmd::cmd_open_brace() {
	parser.read_token();

	if (parser.is_end() && !brackets.empty())
		throw Exception(EXTRA_BRACKET);

	// TODO
	// задать общую функцию
	// например, execute_line
	// при этом можно вынести поток ввода в отдельную переменную
	// и при необходимости переключать ее c cin на file_input
	// точно так же можно поступить с потоком вывода (cout || file_output)

}

void Cmd::cmd_close_brace() {}

void Cmd::cmd_if() {}
void Cmd::cmd_else() {}
void Cmd::cmd_elif() {}
void Cmd::cmd_while() {}
void Cmd::cmd_for() {}
void Cmd::cmd_fun() {}
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

bool Cmd::is_cmd(const std::string& _cmd, int& pos) {
	auto it = cmd_table.find(_cmd);
	if (it != cmd_table.end()) pos = it->second;
	else pos = -1;

	return it != cmd_table.end();
}

bool Cmd::is_return_cmd(int cmd_tok) {
	return return_cmd.find(cmd_tok) != return_cmd.end();
}


void Cmd::define_variable() {
	register char* line = program - token.length();
	std::string var_name(token);
	create_var(var_name);
	
	parser.read_token();
	if (token[0] == ',' || parser.is_end()) return;
	else if (token[0] == '=') {
		parser.putback_token();
		program = line;

		assign_variable();
		parser.read_token();
	}
	else throw Exception(INVALID_SYNTAX);
}


void Cmd::out_string(std::string& result_str) {
	register double result;

	while (true) {
		parser.read_token();
		if (parser.is_end())
			break; // если отсутсвует ) продолжить ввод

		switch (token_type) {
		case QUOTE:
			result_str += token;
			break;
		case FUNCTION:
			break;
		default:
			if (is_cmd(token, tok) && 
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



//////////////////////////////////////////
//////////////////////////////////////////
/////////////				//////////////
/////////////	FUNCTIONS	//////////////
/////////						//////////

Function::Function() : funs() {}

Function::~Function() {}


bool Function::is_fun(const std::string& key) const {
	return funs.find(key) != funs.end();
}

bool Function::is_return_fun(const std::string& key) const {
	auto it = --funs.equal_range(key).second;
	if (it != funs.end())
		return it->second.return_type;
	else throw Exception(UNKNOWN_ERROR);
}


void Function::add_fun(const char* _fun) {

}

void Function::delete_fun(std::string& key) {
	auto it = --funs.equal_range(key).second;
	if (it != funs.end())
		funs.erase(it);
	else throw Exception(UNKNOWN_ERROR);
}

void Function::execute(std::string&) const {

}
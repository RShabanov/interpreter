#include "commands.h"



Cmd cmd;
Executive exec;
Function fun;
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
			//{"}", CLOSE_BRACE},
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
	//case CLOSE_BRACE:
	//	cmd_close_brace();
	//	break;
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

		if (token[0] == ')')
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
				invitational_str += std::to_string(exec.compute_exp());
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
			if (var.is_var(var_name)) var.delete_var(var_name);
		throw e;
	}
}


void Cmd::cmd_open_brace() {
	auto funs = fun.funs;
	std::multimap<std::string, double> vars(var.vars);

	try {
		exec.eval(program); // program???
	}
	catch (Exception& e) {
		fun.funs = funs;
		var.vars = vars;
		throw e;
	}

	fun.funs = funs;
	if (tok != CLOSE_BRACE) {
		var.vars = vars;
		throw Exception(EXTRA_BRACKET);
	}
	del_vars(vars);
}

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
//		AUXILIARY METHODS (CMD)
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
	/*std::string var_name(token);
	create_var(var_name);*/
	var.create_var(token);
	
	parser.read_token();
	if (token[0] == ',' || parser.is_end()) return;
	else if (token[0] == '=') {
		parser.putback_token();
		program = line;

		exec.assign_variable();
		//parser.read_token();
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
			if (is_cmd(token, tok) && is_return_cmd(tok)) {
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
		/*else if (token[0] != ')')
			throw Exception(EXTRA_BRACKET);*/
	}
}

void Cmd::del_vars(std::multimap<std::string, double>& vars) {
	std::vector<std::string> var_name;
	for (auto it : var.vars) var_name.push_back(it.first);

	for (auto name : var_name) {
		auto range = var.vars.equal_range(name);
		auto vars_range = vars.equal_range(name);

		for (auto i = range.first, vi = vars_range.first;
			vi != vars_range.second && i != range.second; 
			i++, vi++) vi->second = i->second;
	}

	var.vars = vars;
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




//////////////////////////////////////////
//////////////////////////////////////////
/////////////				//////////////
/////////////	EXECUTIVE	//////////////
/////////						//////////

void Executive::eval(const char* _code) {
	// отвечает за участок кода, расположенный в { ... },
	// или в глобальной области ????

	/*auto funs = fun.funs;
	std::map<std::string, int> vars;
	get_var_names(vars);*/

	register char* temp = program;
	program = const_cast<char*>(_code);

	try {
		do {
			parser.read_token();

			if (parser.is_eol() || token_type == QUOTE) continue;
			if (parser.is_eof()) break;

			if (token_type == VARIABLE) eval_var();
			else if (fun.is_fun(token)) fun.execute(token);
			else if (cmd.is_cmd(token, tok)) cmd.execute(tok);
			else {
				parser.putback_token();
				register double res = exec.compute_exp();
				parser.read_token();

				if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
			}
		} while (!parser.is_eof() && tok != CLOSE_BRACE);
	}
	catch (Exception& e) {
		program = temp;
		throw e;
	}
	parser.putback_token();
	//_code = program;
	program = temp;

	/*del_vars(vars);
	fun.funs = funs;*/
}


void Executive::assign_variable() const {
	// input: string like "a = b = 5" or "a = 5", etc.

	register double res;

	parser.read_token();
	register std::string target(token);

	parser.read_token();
	if (token[0] == ',') return;
	if (token[0] == '=') {
		parser.read_token();

		if (token_type == VARIABLE)
			res = get_value();
		else {
			parser.putback_token();
			res = compute_exp();
			parser.read_token();

			if (!parser.is_end() && token[0] != ',')
				throw Exception(INVALID_SYNTAX);
		}
		var.assign_var(target, res);
	}
}


double Executive::compute_exp() const {
	// for things like:
	// 2 * input() ...
	// input() * input() - 5 * input(), etc

	std::string exp;
	read_exp(exp);

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


////////////////////////////////////////////
////									////
////	PRIVATE METHODS (EXECUTIVE)		////
////									////
////////////////////////////////////////////

inline bool Executive::not_executive() const {
	return token_type == VARIABLE || token_type == NUMBER ||
		parser.is_expression(token[0]);
}


void Executive::read_exp(std::string& exp) const {
	while (true) {
		parser.read_token();

		// TODO
		// переписать красиво

		if (not_executive()) exp += token;
		else if (cmd.is_cmd(token, tok)) {
			if (cmd.is_return_cmd(tok)) {
				cmd.execute(tok);
				exp += token;
			}
			else if (tok == OPEN_BRACE || tok == CLOSE_BRACE)
				throw Exception(INVALID_SYNTAX);
			else throw Exception(INVALID_TYPE);
		}
		else if (fun.is_fun(token)) {
			if (fun.is_return_fun(token)) {
				fun.execute(token);
				exp += token;
			}
			else if (tok == OPEN_BRACE || tok == CLOSE_BRACE)
				throw Exception(INVALID_SYNTAX);
			else throw Exception(INVALID_TYPE);
		}
		else if (token_type == STRING) // for good exception explanation
			throw Exception(UNDEFINED_NAME);
		else break;

		exp += " ";
	}
	parser.putback_token();
}

double Executive::get_value() const {
	register double res;
	register char* temp = program - token.length();
	register std::string source(token);

	parser.read_token();
	if (parser.is_end()) res = var.get_var(source);
	else {
		parser.putback_token();
		program = temp;
		if (token[0] == '=') {
			assign_variable();
			res = var.get_var(source);
		}
		else {
			res = compute_exp();
			parser.read_token();
			if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
		}
	}
	return res;
}


void Executive::eval_var() const {
	register char* temp = program - token.length();

	parser.read_token();
	parser.putback_token();
	program = temp;

	if (parser.is_expression(token[0]) || parser.is_end()) {
		register double res = exec.compute_exp();
		parser.read_token();

		if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
	}
	else do {
		exec.assign_variable();
	} while (!parser.is_end());
}
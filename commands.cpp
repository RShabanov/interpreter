#include "commands.h"



Executive exec;
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

			if (token_type == STRING && !is_cmd(token, tok) &&
				!fun.is_fun(token)) {
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

void Cmd::cmd_if() {
	// if condition { ... }
	
	bool condition = exec.compute_exp();
	parser.read_token();
	
	if (token[0] == '{') {
		if (condition) {
			exec.eval(program);
			skip_rest_conditional();
		}
		else {
			parser.putback_token();
			skip_if();			
			// to continue in the following branch if there is
			following_branch();
		}
	}
	else throw Exception(INVALID_SYNTAX);
}

void Cmd::cmd_elif() {
	// elif condition { ... }
	if (tok == IF) cmd_if();
	else throw Exception(INVALID_SYNTAX);
}

void Cmd::cmd_else() {
	// else condition { ... }
	if (tok == IF) {
		parser.read_token();

		if (token[0] == '{') exec.eval(program);
		else throw Exception(INVALID_SYNTAX);
	}
	else throw Exception(INVALID_SYNTAX);
}


void Cmd::cmd_while() {
	// while condition { ... }
}

void Cmd::cmd_for() {
	// for let i : range(start, stop, step) { ... }
}

void Cmd::cmd_fun() {
	parser.read_token();
	std::string fun_name(token);

	register char* temp = program;
	int argc = get_params_cnt();

	std::string body(temp, (program - temp) / sizeof(char));
	get_fun_body(body);

	fun.add_fun(fun_name, body, argc);

	token = fun_name; // for while-loop
}

void Cmd::cmd_return() {
	register double res = exec.compute_exp();

	if (parser.is_end() || token[0] == '}') {
		while (*program) {
			if (brackets.is_bracket(*program))
				brackets.push(*program);
			program++;
		}
		token = std::to_string(res);
		tok = RETURN;
	}
	else throw Exception(INVALID_SYNTAX);
}

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
		if (parser.is_end() || token[0] == ')' ||
			token[0] == '}' && !brackets.braces_size())
			break; // если отсутсвует ) продолжить ввод

		if (token_type == QUOTE) result_str += token;
		else {
			parser.putback_token();
			result = exec.compute_exp();

			result_str += std::to_string(result);
		}
		parser.read_token();

		if (token[0] == ',')
			result_str += " ";
		else if (token[0] == ')') break;
	}
}


int Cmd::get_params_cnt() {
	parser.read_token();
	if (token[0] == '(') {
		parser.read_token();
		if (token[0] == ')') return 0;
		if (token == "let") {
			register int cnt = 0;
			do {
				parser.read_token();
				if (token_type == STRING || token_type == VARIABLE)
					cnt++;
				else if (token[0] == ',') {
					parser.read_token();
					if (token_type != STRING && token_type != VARIABLE)
						throw Exception(INVALID_SYNTAX);
					else parser.putback_token();
				}
				else if (token[0] != ')')
					throw Exception(INVALID_SYNTAX);
			} while (token[0] != ')');
			return cnt;
		}
		else throw Exception(INVALID_SYNTAX);
	}
	else throw Exception(EXTRA_BRACKET);
}

void Cmd::get_fun_body(std::string& body) {
	do {
		parser.read_token();
	} while (token[0] == '\r');

	if (token[0] == '{') {
		parser.putback_token();
		register auto braces = brackets.braces_size();
		do {
			parser.read_token();
			if (token_type == QUOTE)
				token = '"' + token + '"';
			body += " " + token;
		} while (!parser.is_eof() &&
			braces != brackets.braces_size());

		if (braces != brackets.braces_size())
			throw Exception(EXTRA_BRACKET);
	}
	else throw Exception(EXTRA_BRACKET);
}

void Cmd::skip_if() {
	exec.compute_exp();
	register auto braces = brackets.braces_size();
	do {
		parser.read_token();
	} while (!parser.is_eof() && 
		braces != brackets.braces_size());

	if (braces != brackets.braces_size())
		throw Exception(EXTRA_BRACKET);
}

void Cmd::skip_rest_conditional() {
	do {
		parser.read_token();
		//if (parser.is_eol()) continue;
	} while (!parser.is_eof() && token_type != STRING);

	if (is_cmd(token, tok) && (tok == ELIF || tok == ELSE)) {
		skip_if();
		skip_rest_conditional();
	}
	else parser.putback_token();
}

void Cmd::following_branch() {
	do {
		parser.read_token();
	} while (!parser.is_eof() && token_type != STRING);

	if (is_cmd(token, tok)) {
		if (tok == ELIF) {
			tok = IF;
			cmd_elif();
		}
		else if (tok == ELSE) {
			tok = IF;
			cmd_else();
		}
		else parser.putback_token();
	}
	else parser.putback_token();
}





//////////////////////////////////////////
//////////////////////////////////////////
/////////////				//////////////
/////////////	EXECUTIVE	//////////////
/////////						//////////

void Executive::eval(const char* _code) {
	// отвечает за участок кода, расположенный в { ... },
	// или в глобальной области ????

	program = const_cast<char*>(_code);

	do {
		parser.read_token();

		if (parser.is_eol() || token_type == QUOTE) continue;
		if (token[0] == '}' && !brackets.braces_size() ||
			parser.is_eof() || tok == RETURN || !*program) 
			break;

		if (token_type == VARIABLE) eval_var();
		else if (fun.is_fun(token)) {
			execute_fun(token);
			if (*program) tok = 0;
		}
		else if (cmd.is_cmd(token, tok)) cmd.execute(tok);
		else {
			parser.putback_token();
			register double res = exec.compute_exp();
			parser.read_token();

			if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
		}
	} while (!parser.is_eof() && tok != RETURN);
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

	register double res = NAN;
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

		if (not_executive()) {
			if (token_type == DELIMITER) invert_opers();
			exp += token;
		}
		else if (cmd.is_cmd(token, tok)) {
			if (cmd.is_return_cmd(tok)) {
				if (tok == RETURN) throw Exception(INVALID_SYNTAX);
				cmd.execute(tok);
				exp += token;
			}
			else throw Exception(INVALID_TYPE);
		}
		else if (fun.is_fun(token)) {
			execute_fun(token);
			if (isdigit(token[0]) || strchr("-+", token[0])) 
				exp += token;
			else throw Exception(INVALID_TYPE);
		}
		else if (token_type == STRING)
			// for good exception explanation
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

void Executive::invert_opers() const {
	switch (token[0]) {
	case EQ:
		token = "==";
		break;
	case NE:
		token = "!=";
		break;
	case GE:
		token = ">=";
		break;
	case LE:
		token = "<=";
		break;
	}
}







//////////////////////////////////////////
//////////////////////////////////////////
/////////////				//////////////
/////////////	FunFunctor	//////////////
/////////						//////////

void FunFunctor::operator()(const std::string& key) {
	auto range = fun.funs.equal_range(key);
	if (range.first != range.second) {
		std::vector<double> values;
		read_param_value(values);

		for (auto it = range.first; it != range.second; it++)
			if (it->second.argc == values.size()) {
				register char* temp = program;

				try {
					program = const_cast<char*>(it->second.body.c_str());
					execute(values);
					program = temp;
					return;
				}
				catch (Exception& e) {
					program = temp;
					throw e;
				}
			}
		throw Exception(UNDEFINED_NAME);
	}
	else throw Exception(UNDEFINED_NAME);
}

void FunFunctor::execute(std::vector<double>& values) {
	std::multimap<std::string, double> vars;
	var.copy_to(vars);

	auto temp_funs(fun.funs);

	try {
		add_fun_vars(values);
		do {
			parser.read_token();
		} while (parser.is_eol());

		if (token[0] == '{') exec.eval(program);
		else throw Exception(EXTRA_BRACKET);

		// TODO
		// восстанавливать функции нужно осторожно
		fun.funs = temp_funs;
		var.restore_with_changes(vars);
		tok = RETURN;
	}
	catch (Exception& e) {
		var.restore(vars);
		fun.funs = temp_funs;
		throw e;
	}
}

void FunFunctor::add_fun_vars(std::vector<double>& values) {
	// сопоставляет переменные со значениями
	// (let a, b, c)
	// (	1, 2, 3), e.g.
	// т.к. проверка на корректность формы
	// происходит перед данной функцией,
	// то здесь ее можно опустить

	parser.read_token(); // skip '('
	parser.read_token(); // skip "let" if it is
	if (token[0] == ')') return;

	for (size_t i = 0; i < values.size(); i++) {
		parser.read_token();
		var.create_var(token, values.at(i));

		parser.read_token(); // read ',' or ')';
	}
}

void FunFunctor::read_param_value(std::vector<double>& values) {
	parser.read_token();
	if (token[0] == '(') {
		register double val;
		do {
			val = exec.compute_exp();
			parser.read_token();
			if (std::isnan(val)) {
				if (token[0] == ')') break;
				else throw Exception(INVALID_TYPE);
			}
			values.push_back(val);
		} while (token[0] == ',');
	}
	else throw Exception(EXTRA_BRACKET);
}
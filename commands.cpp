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
			{"break", BREAK},
			{"fun", FUN},
			{"let", LET},
			{"and", AND},
			{"or", OR},
			{"not", NOT},
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
				invitational_str += std::to_string(exec.compute_expr());
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
	bool condition = exec.compute_expr();

	parser.skip_eol();
	parser.read_token();

	if (token[0] == '{') {
		std::multimap<std::string, double> vars;
		var.copy_to(vars);

		try {
			if (condition) {
				exec.eval();
				skip_rest_conditional();
			}
			else {
				parser.putback_token();
				skip_executive_block();
				// to continue in the following branch if there is
				following_branch();
			}
			var.restore_with_changes(vars);
		}
		catch (Exception& e) {
			var.restore(vars);
			throw e;
		}
	}
	else throw Exception(INVALID_SYNTAX);
}

void Cmd::cmd_elif() {
	// elif condition { ... }
	if (tok == IF) cmd_if();
	else throw Exception(ELIF_WITHOUT_IF);
}

void Cmd::cmd_else() {
	// else condition { ... }
	if (tok == IF) {
		parser.skip_eol();
		parser.read_token();

		if (token[0] == '{') {
			std::multimap<std::string, double> vars;
			var.copy_to(vars);

			try {
				exec.eval();
				var.restore_with_changes(vars);
			}
			catch (Exception& e) {
				var.restore(vars);
				throw e;
			}
		}
		else throw Exception(INVALID_SYNTAX);
	}
	else throw Exception(ELSE_WITHOUT_IF);
}



void Cmd::cmd_while() {
	// while condition { ... }
	register char* loop_start = program;
	bool condition = exec.compute_expr();

	if (condition) {
		std::multimap<std::string, double> vars;
		var.copy_to(vars);

		try {
			do {
				parser.skip_eol();
				parser.read_token();
				if (token[0] != '{')
					throw Exception(INVALID_SYNTAX);

				exec.eval();
				program = loop_start;

				var.restore_with_changes(vars);
			} while (exec.compute_expr());
		}
		catch (Exception& e) {
			var.restore(vars);
			throw e;
		}
	}
	else skip_executive_block();
}

void Cmd::cmd_for() {
	// for let i : range(start, stop, step) { ... }
}

void Cmd::cmd_break() {

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
	register double res = exec.compute_expr();

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
	if (parser.is_comma(token[0]) || parser.is_end()) return;
	else if (token[0] == '=') {
		parser.putback_token();
		program = line;

		exec.assign_variable();
	}
	else throw Exception(INVALID_SYNTAX);
}


void Cmd::out_string(std::string& result_str) {
	register double result;

	while (true) {
		parser.read_token();
		if (parser.is_end() || token[0] == ')' ||
			token[0] == '}' && !brackets.braces_size())
			break;

		if (token_type == QUOTE) result_str += token;
		else {
			parser.putback_token();
			result = exec.compute_expr();

			result_str += std::to_string(result);
		}
		parser.read_token();

		if (parser.is_comma(token[0]))
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
				else if (parser.is_comma(token[0])) {
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
	parser.skip_eol();
	parser.read_token();


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

void Cmd::skip_executive_block() {
	if (is_cmd(token, tok) && tok == ELIF)
		exec.compute_expr();

	parser.skip_eol();
	parser.read_token();

	if (token[0] == '{') {
		register auto braces = brackets.braces_size() - 1;
		do {
			parser.read_token();
		} while (!parser.is_eof() &&
			braces != brackets.braces_size());

		if (braces != brackets.braces_size())
			throw Exception(EXTRA_BRACKET);
	}
	else throw Exception(EXTRA_BRACKET);
}

void Cmd::skip_rest_conditional() {
	jump_to_another_word();

	if (is_cmd(token, tok) && (tok == ELIF || tok == ELSE)) {
		skip_executive_block();
		skip_rest_conditional();
	}
	else parser.putback_token();
}

void Cmd::following_branch() {
	jump_to_another_word();

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

void Cmd::jump_to_another_word() {
	do {
		parser.read_token();
	} while (!parser.is_eof() && token_type != STRING);
}

bool Cmd::is_logic_oper(int _tok) {
	if (_tok == AND || _tok == OR || _tok == NOT)
		return true;
	return false;
}





//////////////////////////////////////////
//////////////////////////////////////////
/////////////				//////////////
/////////////	EXECUTIVE	//////////////
/////////						//////////

void Executive::eval() {
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
			exec.compute_expr();
			parser.read_token();

			if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
		}
	} while (!parser.is_eof() && tok != RETURN);
}


void Executive::assign_variable() {
	// input: string like "a = b = 5" or "a = 5", etc.

	register double res;

	parser.read_token();
	register std::string target(token);

	parser.read_token();
	if (parser.is_comma(token[0])) return;
	if (token[0] == '=') {
		parser.read_token();

		if (token_type == VARIABLE)
			res = get_value();
		else {
			parser.putback_token();
			res = compute_expr();
			parser.read_token();

			if (!parser.is_end() && token[0] != ',')
				throw Exception(INVALID_SYNTAX);
		}
		var.assign_var(target, res);
	}
}


double Executive::compute_expr() {
	// for things like:
	// 2 * input() ...
	// input() * input() - 5 * input(), etc

	size_t parenthesis_cnt = 0;
	std::string temp, expr;

	while (true) {
		parser.read_token();

		if (parser.is_expr_end()) break;

		if (brackets.is_parenthesis(token[0])) {
			parenthesis_cnt += token[0] == '(' ? 1 : -1;
			if (parenthesis_cnt == SIZE_MAX &&
				brackets.parenthesis_size() >= 0)
				break;

			temp += token;
		}
		else if (not_executive()) {
			if (token_type == DELIMITER) invert_opers();
			temp += token;
		}
		else if (cmd.is_cmd(token, tok)) {
			if (cmd.is_return_cmd(tok)) {
				if (tok == RETURN)
					throw Exception(INVALID_SYNTAX);
				cmd.execute(tok);
				temp += token;
			}
			else if (cmd.is_logic_oper(tok)) {
				if (tok != NOT)
					temp = "(" + temp + ")" + (tok == AND ? "*" : "+");
				else
					temp += "(" + std::to_string(!get_condition_not()) + ")";

				expr += temp;
				temp.clear();
				continue;
			}
			else throw Exception(INVALID_TYPE);
		}
		else if (fun.is_fun(token)) {
			execute_fun(token);
			if (contains_number(token[0]))
				temp += token;
			else throw Exception(INVALID_TYPE);
		}
		else if (token_type == STRING)
			throw Exception(UNDEFINED_NAME);
		else break;

		temp += " ";
	}
	parser.putback_token();

	if (contains_alnum(temp))
		expr += "(" + temp + ")";
	else expr += temp;

	return get_number(expr);
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


bool Executive::get_condition_not() {
	register size_t parenthesis_cnt = 0;
	register bool has_logic_oper = false;
	std::string expr, temp;

	while (true) {
		parser.read_token();

		if (parser.is_expr_end()) break;

		if (brackets.is_parenthesis(token[0])) {
			parenthesis_cnt += token[0] == '(' ? 1 : -1;
			if (has_logic_oper && !parenthesis_cnt ||
				parenthesis_cnt == SIZE_MAX) break;

			temp += token;
		}
		else if (not_executive()) {
			if (token_type == DELIMITER) invert_opers();
			temp += token;
		}
		else if (cmd.is_cmd(token, tok)) {
			if (cmd.is_return_cmd(tok)) {
				if (tok == RETURN)
					throw Exception(INVALID_SYNTAX);
				cmd.execute(tok);
				temp += token;
			}
			else if (cmd.is_logic_oper(tok)) {
				if (tok == NOT) 
					temp += "(" + std::to_string(!get_condition_not()) + ")";
				else {
					if (parenthesis_cnt)
						temp = "(" + temp + ")" + (tok == AND ? "*" : "+");
					else break;
				}

				has_logic_oper = true;
				expr += temp;
				temp.clear();
				continue;
			}
			else throw Exception(INVALID_TYPE);
		}
		else if (fun.is_fun(token)) {
			execute_fun(token);
			if (contains_number(token[0]))
				temp += token;
			else throw Exception(INVALID_TYPE);
		}
		else if (token_type == STRING)
			throw Exception(UNDEFINED_NAME);
		else break;

		temp += " ";
	}
	parser.putback_token();
	if (contains_alnum(temp)) 
		expr += "(" + temp + ")";
	else expr += temp;

	return get_number(expr);
}


double Executive::get_number(std::string& expr) {
	if (expr.empty()) throw Exception(INVALID_SYNTAX);

	register double res = NAN;
	register char* temp_start = program;

	try {
		program = const_cast<char*>(expr.c_str());
		parser.parse(res);
	}
	catch (Exception& e) {
		program = temp_start;
		throw e;
	}

	program = temp_start;
	return res;
}



double Executive::get_value() {
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
			res = compute_expr();
			parser.read_token();
			if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
		}
	}
	return res;
}


void Executive::eval_var() {
	register char* temp = program - token.length();

	parser.read_token();
	parser.putback_token();
	program = temp;

	if (parser.is_expression(token[0]) || parser.is_end()) {
		register double res = exec.compute_expr();
		parser.read_token();

		if (!parser.is_end()) throw Exception(INVALID_SYNTAX);
	}
	else do {
		exec.assign_variable();
	} while (!parser.is_end());
}

void Executive::invert_opers() {
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
	case AND:
		token = "&&";
		break;
	case OR:
		token = "||";
		break;
	}
}

inline bool Executive::contains_number(char symbol) const {
	return isdigit(symbol) || symbol == '-' ||
		symbol == '+';
}

inline bool Executive::contains_alnum(std::string& expr) const {
	for (auto symbol : expr)
		if (isalnum(symbol))
			return true;
	return false;
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
					program = const_cast<char*>(it->second.body->c_str());
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

		parser.skip_eol();
		parser.read_token();

		if (token[0] == '{') exec.eval();
		else throw Exception(EXTRA_BRACKET);

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
			val = exec.compute_expr();
			parser.read_token();
			if (std::isnan(val)) {
				if (token[0] == ')') break;
				else throw Exception(INVALID_TYPE);
			}
			values.push_back(val);
		} while (parser.is_comma(token[0]));
	}
	else throw Exception(EXTRA_BRACKET);
}
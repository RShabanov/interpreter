#include "Parser.h"


bool is_end() {
	return tok == EOL || tok == FINISHED;
}

// -------------------------------
Parser parser;

Parser::Parser() {
	//token_type = 0;
	//prog_start = program = 0;
}

Parser::~Parser() {}


void Parser::parse(double& result) {
	read_token();
	if (!token.empty()) {
		parse_relation_oper(result);
		putback_token();
	}
}


void Parser::parse_relation_oper(double& result) {
	parse_add_sub(result);

	char relation_ops[] = { EQ, NE,GE,LE,'>','<',0 };
	register char oper = token[0];

	if (strchr(relation_ops, oper) && oper != '\0') {
		double temp;

		read_token();
		parse_relation_oper(temp);
		switch (oper) {
		case '<':
			result = result < temp;
			break;
		case LE:
			result = result <= temp;
			break;
		case '>':
			result = result > temp;
			break;
		case GE:
			result = result >= temp;
			break;
		case EQ:
			result = result == temp;
			break;
		case NE:
			result = result != temp;
			break;
		}
	}
}


void Parser::parse_add_sub(double& result) {
	parse_mul_div(result);

	register char oper = token[0];
	
	while (oper == '+' || oper == '-') {
		double temp;

		read_token();
		parse_mul_div(temp);

		switch (oper) {
		case '-':
			result -= temp;
			break;
		case '+':
			result += temp;
			break;
		}

		oper = token[0];
	}
}


void Parser::parse_mul_div(double& result) {
	parse_power(result);

	register char oper = token[0];

	while (oper == '*' || oper == '/') { // || oper == '%'
		double temp;

		read_token();
		parse_power(temp);

		switch (oper) {
		case '/':
			result /= temp;
			break;
		case '*':
			result *= temp;
			break;
		case '%':
			// TODO
			// реализовать операцию деления по модулю
			break;
		}

		oper = token[0];
	}
}


void Parser::parse_power(double& result) {
	parse_unary(result);
	
	if (token[0] == '^') {
		double power;

		read_token();
		parse_power(power);

		result = std::pow(result, power);
	}
}


void Parser::parse_unary(double& result) {
	register char oper = 0;

	if (token_type == DELIMITER && token[0] == '+' || token[0] == '-') {
		oper = token[0];
		read_token();
	}

	parse_brackets(result);

	if (oper == '-') result = -result;
}


void Parser::parse_brackets(double& result) {
	if (token[0] == '(') {
		read_token();
		parse_relation_oper(result);

		if (token == "=") throw ParserException(INVALID_SYNTAX);
		if (token[0] != ')') throw ParserException(EXTRA_BRACKET);
		
		read_token();
	}
	else read_values(result);
}



void Parser::read_values(double& result) {
	switch (token_type) {
	case VARIABLE:
		result = get_var(token);
		read_token();
		break;
	case NUMBER:
		result = atof(token.c_str());
		read_token();
		break;
	case STRING: throw VariableException(UNDEFINED_NAME);
	default: throw ParserException(INVALID_SYNTAX);
	}
}


// чтение следующего токена
void Parser::read_token() {
	token.clear();
	token_type = 0;
	tok = 0;

	skip_space();

	if (is_eof(program)) token_eof();
	else if (is_cr(program)) token_cr();
	else if (is_opers(program)) token_opers();
	else if (is_delim(program)) token_delim();
	else if (is_quote(program)) token_quote();
	else if (is_number(program)) token_number();
	else if (isalpha(*program)) {
		token_string();

		if (is_var(token)) token_type = VARIABLE;
		else if (is_function(token)) token_type = FUNCTION;
		else token_type = STRING;

		/*if (is_cmd(token.c_str(), tok)) token_type = COMMAND;
		else if (is_function(token.c_str(), tok)) 
			token_type = FUNCTION;
		else token_type = VARIABLE;*/
	}
}


inline bool Parser::is_eof(char* symbol) const {
	return *symbol == '\0';
}

inline bool Parser::is_space(char* symbol) const {
	return *symbol == ' ' || *symbol == '\t';
}

inline bool Parser::is_cr(char* symbol) const {
	return *symbol == '\r';
}

inline bool Parser::is_opers(char* symbol) const {
	return strchr("<!=>", *symbol);
}

inline bool Parser::is_delim(char* symbol) const {
	return strchr("+-*^/=;(),", *symbol);
}

inline bool Parser::is_quote(char* symbol) const {
	return *symbol == '"';
}

inline bool Parser::is_number(char* str) const {
	register char* temp = str;
	register bool fst_digit = false;

	while (*temp && !strchr("+-*^/=;(), \t\r", *temp)) {
		if (*temp != '.' && !isdigit(*temp)) {
			if (fst_digit && isalpha(*temp)) 
				throw ParserException(INVALID_SYNTAX);
			return false;
		}
		temp++;
		fst_digit = true;
	}
	return true;
}

inline bool Parser::is_escape_char(char* str) const {
	register char* temp = str;
	if (*temp++ == '\\')
		switch (*temp) {
		case 'r':
		case 'n':
		case 'v':
		case 't':
		case 'b':
		case 'f':
		case '0':
			return true;
		}
	return false;
}


inline void Parser::skip_space() {
	while (is_space(program)) program++;
}



void Parser::token_eof() {
	token_type = DELIMITER;
	tok = FINISHED;
}

void Parser::token_cr() {
	program += 2;
	token.push_back('\r');
	token.push_back('\n');
	token_type = DELIMITER;
	tok = EOL;
}

void Parser::token_opers() {
	switch (*program) {
	case '<':
		if (*(program + 1) == '=') {
			program += 2;
			token.push_back(LE);
		}
		else {
			program++;
			token.push_back('<');
		}
		break;
	case '>':
		if (*(program + 1) == '=') {
			program += 2;
			token.push_back(GE);
		}
		else {
			program++;
			token.push_back('>');
		}
		break;
	case '=':
		if (*(program + 1) == '=') {
			program += 2;
			token.push_back(EQ);
		}
		else token_delim();
		break;
	case '!':
		if (*(program + 1) == '=') {
			program += 2;
			token.push_back(NE);
		}
		else throw ParserException(INVALID_SYNTAX);
		break;
	}

	token_type = DELIMITER;
}

void Parser::token_delim() {
	if (*program == '(') parentheses.push_back('(');
	else if (*program == ')') {
		if (parentheses.empty())
			throw ParserException(EXTRA_BRACKET);
		parentheses.pop_back();
	}
	
	token.push_back(*program);
	program++;
	token_type = DELIMITER;
}

void Parser::token_quote() {
	program++;
	while (*program != '"' && *program != '\r') {
		if (is_escape_char(program)) {
			token.push_back(get_escape_char(program + 1));
			program += 2;
		}
		else token.push_back(*program++);
	}
	if (*program == '\r') throw ParserException(MISS_QUOTE);
	program++;
	token_type = QUOTE;
}

void Parser::token_number() {
	while (!strchr("+-*^/<!=>;(),\r \t", *program))
		token.push_back(*program++);
	token_type = NUMBER;
}

void Parser::token_string() {
	while (!strchr(" \t; ,+=<>/*%^()\r\"", *program)) { // Читаем до тех пор пока не встретим разделитель " ; ,+=<>/*%^()"
		token.push_back(*program++);
	}
}


void Parser::putback_token() {
	const char* t = token.c_str();
	while (*t) {
		if (*t == '(' && !parentheses.empty()) 
			parentheses.pop_back();
		else if (*t == ')') parentheses.push_back(')');
		program--;
		t++;
	}
}


char Parser::get_escape_char(char* _str) const {
	switch (*_str) {
	case 'r': return '\r';
	case 'n': return '\n';
	case 'v': return '\v';
	case 't': return '\t';
	case 'b': return '\b';
	case 'f': return '\f';
	case '0': return '\0';
	default: return *_str;
	}
}
#include "Parser.h"


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

		if (token == "=") throw Exception(INVALID_SYNTAX);
		if (token[0] != ')') throw Exception(EXTRA_BRACKET);
		
		read_token();
	}
	else read_values(result);
}



void Parser::read_values(double& result) {
	switch (token_type) {
	case VARIABLE:
		result = var.get_var(token);
		break;
	case NUMBER:
		result = atof(token.c_str());
		break;
	case STRING: throw Exception(UNDEFINED_NAME);
	default: throw Exception(INVALID_SYNTAX);
	}

	read_token();
	if (token_type != DELIMITER)
		throw Exception(INVALID_SYNTAX);
}


// чтение следующего токена
void Parser::read_token() {
	token.clear();
	token_type = 0;
	tok = 0;

	skip_space(); // добавить сюда экранированные символы ???

	if (is_eof(*program)) token_eof();
	else if (is_cr(*program)) token_cr();
	else if (is_opers(*program)) token_opers();
	else if (is_delim(*program)) token_delim();
	else if (is_quote(*program)) token_quote();
	else if (is_number(program)) token_number();
	else if (is_string(*program)) {
		token_string();

		if (var.is_var(token)) token_type = VARIABLE;
		else token_type = STRING;
	}
	else if (is_comment(*program)) token_comment();
	else throw Exception(INVALID_SYNTAX);
}


inline bool Parser::is_eof(char symbol) const {
	return symbol == '\0';
}

inline bool Parser::is_space(char symbol) const {
	return symbol == ' ' || symbol == '\t';
}

inline bool Parser::is_cr(char symbol) const {
	return symbol == '\r';
}

inline bool Parser::is_opers(char symbol) const {
	return strchr("<!=>", symbol);
}

inline bool Parser::is_delim(char symbol) const {
	return strchr("+-*^/=;(){},", symbol);
}

inline bool Parser::is_quote(char symbol) const {
	return symbol == '"';
}

inline bool Parser::is_number(char* str) const {
	register char* temp = str;

	if (isdigit(*temp)) {
		while (isdigit(*++temp));
		if (*temp == '.') while (isdigit(*++temp));

		if (*temp != '\0' && (isalpha(*temp) || strchr("._", *temp)))
			throw Exception(INVALID_SYNTAX);
		return true;
	}
	else return false;
}

inline bool Parser::is_string(char s) const {
	return isalpha(s) || s == '_';
}

inline bool Parser::is_comment(char s) const {
	return s == '#';
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
		case '\"':
			return true;
		}
	return false;
}


inline void Parser::skip_space() {
	while (is_space(*program)) program++;
}

bool Parser::is_expression(char symbol) {
	const char opers[] = { '+','-','/','*','^','>','<', EQ, NE, GE, LE, 0 };
	return strchr(opers, symbol) && symbol != '\0';
}

bool Parser::is_end() const {
	return tok == EOL || tok == FINISHED;
}

bool Parser::is_eof() const {
	return tok == FINISHED;
}

bool Parser::is_eol() const {
	return tok == EOL;
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
		else token.push_back(*program++);
		break;
	case '>':
		if (*(program + 1) == '=') {
			program += 2;
			token.push_back(GE);
		}
		else token.push_back(*program++);
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
		else throw Exception(INVALID_SYNTAX);
		break;
	}

	token_type = DELIMITER;
}

void Parser::token_delim() {
	if (brackets.is_bracket(*program))
		brackets.push(*program);
	
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
	if (*program == '\r') throw Exception(MISS_QUOTE);
	program++;
	token_type = QUOTE;
}

void Parser::token_number() {
	while (isdigit(*program) || *program == '.')
		token.push_back(*program++);

	token_type = NUMBER;
}

void Parser::token_string() {
	while (isalnum(*program) || *program == '_')
		token.push_back(*program++);
}

void Parser::token_comment() {
	do {
		program++;
	} while (*program != '\0' && *program != '\r');
	parser.read_token();
}


void Parser::putback_token() {
	const char* t = token.c_str();
	while (*t) {
		if (brackets.is_bracket(*t)) brackets.putback(*t);

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
	case '\"': return '\"';
	default: return *_str;
	}
}


void Parser::skip_eol() {
	do {
		read_token();
	} while (is_eol());
	putback_token();
}
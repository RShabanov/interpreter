#include "Parser.h"


Commands cmd_table[] = {
	"print", PRINT,
	"input", INPUT,
	"if", IF,
	"else", ELSE,
	"elif", ELIF,
	"while", WHILE,
	"for", FOR,
	"def", DEF,
	"return", RETURN,
	":", COLON,
	"", END
};


Parser::Parser(char* _prog) {
	//token = 0;
	//token = new char[80];
	token_type = 0;
	program = _prog;
}

Parser::~Parser() {
	//if (token != 0) delete[] token;
}


void Parser::parse(double& result) {
	read_token();
	if (!token.empty()) {
		parse_relation_oper(result);
		// функция по возврату символа во вводный поток
		// ???
		//putback();
	}
}


void Parser::parse_relation_oper(double& result) {
	parse_add_sub(result);

	char relation_ops[] = { EQ, NE,GE,LE,'>','<',0 };
	register char oper = token[0];

	if (strchr(relation_ops, oper)) {
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
	
	if (oper == '+' || oper == '-') {
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
	}
}


void Parser::parse_mul_div(double& result) {
	parse_power(result);

	register char oper = token[0];

	if (oper == '*' || oper == '/') {
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
		}
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
		// ???
		// потому что это странно,
		// что мы не учитываем такую возможность:
		// (a >= b), например
		// 
		//parse_relation_oper(result);
		parse_add_sub(result);

		if (token[0] != ')') throw ParserException(EXTRA_BRACKET);
		read_token();
	}
	else read_values(result);
}




void Parser::read_values(double& result) {
	switch (token_type) {
	case VARIABLE:
		result = find_variable();
		read_token();
		break;
	case NUMBER:
		result = atof(&token[0]);
		read_token();
		break;
	case FUNCTION:
		// TODO
		// реализовать чтение функции
		break;
	default: throw ParserException(INVALID_SYNTAX);
	}
}


double Parser::find_variable() const {
	// TODO
	// реализовать поиск переменных в массиве
	// желательно реализовать поиск в отсортированном по алфавиту массиве
	// 
	// поиск осуществлять с конца
	//
	// или
	// данная функция должна находиться с другом месте
	// например, в классе init или каком-то подобном
	return 0;
}



// чтение следующего токена
void Parser::read_token() {
	register char* temp = &token[0];
	token_type = 0;

	if (is_eof()) {
		token_eof();
		return;
	}
	while (is_space()) program++;

	if (is_cr()) token_cr();
	else if (is_opers()) token_opers(temp);
	else if (is_delim()) token_delim(temp);
	else if (is_quote()) token_quote(temp);
	else if (isdigit(*program)) token_number(temp);
	else if (isalpha(*program)) {
		token_string(temp);

		//Не является ли строка командой или переменной
		if (token_type == STRING) {
			tok = find_cmd();
			if (!tok) token_type = VARIABLE;
			// TODO 
			// добавить проверку на функцию
			else token_type = COMMAND;
		}
	}
	//*temp = '\0';
}


inline bool Parser::is_eof() const {
	return *program == '\0';
}

inline bool Parser::is_space() const {
	return *program == ' ' || *program == '\t';
}

inline bool Parser::is_cr() const {
	return *program == '\r';
}

inline bool Parser::is_opers() const {
	return strchr("<!=>", *program);
}

inline bool Parser::is_delim() const {
	return strchr("+-*^/=;(),", *program);
}

inline bool Parser::is_quote() const {
	return *program == '"';
}


void Parser::token_eof() {
	//*token = 0;
	token_type = DELIMITER;
	tok = FINISHED;
}

void Parser::token_cr() {
	program += 2;
	/**token = '\r';
	token[1] = '\n';*/
	token.push_back('\r');
	token.push_back('\n');
	token_type = DELIMITER;
	tok = EOL;
}

void Parser::token_opers(char* _temp) {
	switch (*program) {
	case '<':
		if (*(program + 1) == '=') {
			program += 2;
			*_temp = LE;
		}
		else {
			program++;
			*_temp = '<';
		}
		break;
	case '>':
		if (*(program + 1) == '=') {
			program += 2;
			*_temp = GE;
		}
		else {
			program++;
			*_temp = '>';
		}
		break;
	case '=':
		if (*(program + 1) == '=') {
			program += 2;
			*_temp = EQ;
		}
		else return;
		break;
	case '!':
		if (*(program + 1) == '=') {
			program += 2;
			*_temp = NE;
		}
		else throw ParserException(INVALID_SYNTAX);
		break;
	}
	
	_temp++;
	*_temp = '\0';
	token_type = DELIMITER;
}

void Parser::token_delim(char* _temp) {
	*_temp = *program;
	program++;
	_temp++;
	*_temp = 0;
	token_type = DELIMITER;
}

void Parser::token_quote(char* _temp) {
	program++;
	while (*program != '"' && *program != '\r')
		*_temp++ = *program++;
	if (*program == '\r') throw ParserException(MISS_QUOTE);
	program++;
	*_temp = 0;
	token_type = QUOTE;
}

void Parser::token_number(char* _temp) {
	while (!is_delim()) *_temp++ = *program++;
	//*_temp = '\0';
	token_type = NUMBER;
}

void Parser::token_string(char* _temp) {
	while (!is_delim()) // Читаем до тех пор пока не встретим разделитель " ; ,+=<>/*%^()"
		*_temp++ = *program++;
	token_type = STRING;
}


int Parser::find_cmd() const {
	for (int i = 0; *cmd_table[i].command; i++)
		if (!strcmp(cmd_table[i].command, &token[0]))
			return cmd_table[i].tok;
	return 0; // неизвестная команда
}

#include "init.h"


// TODO
// реализовать свои типы данных???
//
// подумать над реализовацией чтения\сохранения кода
//
// вместо : использовать {} (braces)
//
// может, создать общий класс ошибок, 
// от которого будут наследоваться ошибки для нужного файла
//
// sin, cos, log, ect




Init::Init() {}


Init::~Init() {}


void Init::run() {
	using std::cin;
	using std::cout;
	using std::endl;
	using std::getline;

	std::string line, str;
	double res;
	
	cout << "> ";

	// TODO
	// если назвать переменную так же, как команду,
	// то команда становится недоступной

	while (true) {
		getline(cin, line);
		if (line == "!q") break;
		if (line == "--help") {
			print_help();
			cout << "> ";
			continue;
		}
		try {
			if (!line.empty()) {
				str += line;
				reset(str);

				parser.read_token();

				if (token_type == QUOTE)
					cout << token << endl;
				else if (token_type == VARIABLE)
					token_variable();
				else if (token_type == FUNCTION) {
					// function_act
				}
				else if (cmd.is_cmd(token.c_str(), tok)) {
					cmd.execute(tok);
				}
				else {
					token_expression();
				}
			}
		}
		catch (ParserException& e) {
			if (e.type() != EXTRA_BRACKET || parentheses.empty()) {
				cout << e.what() << endl;
			}
			else {
				cout << "\t";
				continue;
			}
		}
		catch (VariableException& e) {
			cout << e.what() << endl;
		}
		catch (...) {
			cout << "Something went wrong.\n"
				<< "Run-time error.\n";
		}
		str.clear();
		cout << "> ";
	}
}


void Init::reset(std::string& _str) const {
	program = prog_start = &_str[0];
	token_type = tok = 0;
	parentheses.clear();
}

void Init::print_help() const {
	using std::cout;
	using std::endl;

	cout << endl
		<< "This interpreter uses Python's syntax, except for lots of stuff." << endl
		<< "Here are the main of ones (remember, it is a pared version):" << endl
		<< "\t* power (instead of \"**\" you can use \"^\"." << endl
		<< "\t* to define variable use word \"let\", e.g., let my_var = 5." << endl
		<< "\t\t- Variables always have to be initialized" << endl
		<< "\t\t- To define more than one variable use this syntax: let my_var1 = 5, my_var2 = 3" << endl
		<< "\t* instead of tabs and colons use \"{}\"." << endl
		<< "\t  By the way, they must be all the time you use constructions that have them" << endl
		<< "\t* to close this interpreter type \"!q\"" << endl
		<< endl;
}


void Init::token_variable() const {
	// TODO
	// можно ли написать это более красиво???
	// 
	// Problems:
	// a = 3, b (может, оставить???)
	// a = b (лишний вывод) !!!

	register char* temp = program - token.length();

	parser.read_token();
	if (strchr("+-/*^", token[0]) ||
		tok == EOL || tok == FINISHED) { // %
		program = temp;
		//token_expression();
		double res;
		parser.parse(res);

		if (tok != EOL && tok != FINISHED)
			throw ParserException(INVALID_SYNTAX);

		std::cout << res << std::endl;
		return;
	}
	else {
		parser.putback_token();
		define_variable();
	}



	/*register size_t var_index = tok;
	register char* start = program - token.length();

	parser.read_token();
	if (token[0] == '=') {
		parser.read_token();
		if (token_type == VARIABLE) {
			register size_t temp_var_index = tok;
			token_variable();

			change_var_value(var_index, get_variable(temp_var_index));
		}
		else {
			double res;
			parser.putback_token();
			parser.parse(res);

			change_var_value(var_index, res);
		}
	}
	else if (token[0] != ',') {
		program = start + token.length();
		token_expression();
		return;
	}

	if (token[0] == ',') {
		parser.read_token();
		token_variable();
	}
	else if (tok != EOL || tok != FINISHED)
		throw ParserException(INVALID_SYNTAX);*/

}


void Init::token_expression() const {
	double res;

	parser.putback_token();
	parser.parse(res);

	if (tok != EOL && tok != FINISHED)
		throw ParserException(INVALID_SYNTAX);

	std::cout << res << std::endl;
}


void Init::define_variable() const {
	register size_t var_index = tok;

	parser.read_token();

	if (token[0] == '=') {
		register char* ptr = program;
		parser.read_token();

		double res;
		
		if (token_type == VARIABLE) {
			register size_t temp = tok;
			/*register char* ptr = program;

			parser.read_token();
			if (token[0] != '=' && tok != EOL && tok != FINISHED)
				throw ParserException(INVALID_SYNTAX);

			program = ptr;*/
			program = ptr;
			parser.parse(res);

			register char oper = token[0];
			register char* t = program;
			program = ptr;

			parser.read_token();
			parser.read_token();

			if (program != t && oper == '=')
				throw ParserException(INVALID_SYNTAX);
				
			if (oper == '\0') {
				program = t;
				token_type = DELIMITER;
				tok = EOL;
				change_var_value(var_index, res);
			}
			else {
				define_variable();
				change_var_value(var_index, get_variable(temp));
			}


		}
		else {
			parser.putback_token();
			parser.parse(res);
			parser.read_token();

			if (token[0] != '=')
				change_var_value(var_index, res);
			else throw ParserException(INVALID_SYNTAX);
		}
	}
	
	if (token[0] == ',') {
		parser.read_token();
		if (token_type == VARIABLE)
			define_variable();
		else throw ParserException(INVALID_SYNTAX);
	}
	else if (tok != EOL && tok != FINISHED)
		throw ParserException(INVALID_SYNTAX);



	//do {
	//	parser.read_token();

	//	if (token[0] == '=') {
	//		double res;
	//		register char* temp = program;

	//		parser.read_token();
	//		if (token_type == VARIABLE)
	//		//parser.parse(res);

	//		change_var_value(var_index, res);
	//	}
	//	else if (token[0] == ',') {
	//		parser.read_token();
	//		if (token_type == VARIABLE) {
	//			//var_index = tok;
	//			define_variable();
	//		}
	//		else throw ParserException(INVALID_SYNTAX);
	//	}
	//	else throw ParserException(INVALID_SYNTAX);

	//} while (tok != EOL && tok != FINISHED);
}
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
					parser.putback_token();
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
	program = prog_start = const_cast<char*>(_str.c_str());
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
	register char* temp = program - token.length();
	std::string name(token);

	parser.read_token();
	parser.putback_token();

	program = temp;
	const char opers[] = { '+','-','/','*','^','>','<', EQ, NE, GE, LE, 0 };

	if (strchr(opers, token[0]) || is_end()) // %
		token_expression();
	else if ((token == "(" || isalpha(token[0])) &&
		cmd.is_cmd(name.c_str(), tok)) {
		// если имя переменной совпадает с командой

		register int cmd_tok = tok;
		if (!parentheses.empty()) 
			parentheses.pop_back(); // вернуть баланс скобок

		parser.read_token();
		cmd.execute(cmd_tok);
		// если имя переменной совпадает с командой
	}
	else do {
		assign_variable();
		parser.read_token();
	} while (!is_end());
}


void Init::token_expression() const {
	register double res;
	parser.parse(res);

	if (!is_end()) throw ParserException(INVALID_SYNTAX);

	std::cout << res << std::endl;
}
#include "scanner.h"


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




Scanner::Scanner() {}


Scanner::~Scanner() {}


void Scanner::run() {
	using std::cin;
	using std::cout;
	using std::endl;
	using std::getline;

	std::string line, str;
	double res = 0;
	
	cout << "This variant (real-time) of interpreter is still on work" << endl
		<< "That's why something can work otherwise or strange" << endl
		<< endl;
	cout << "> ";

	// TODO
	// если назвать переменную так же, как команду,
	// то команда становится недоступной

	while (true) {
		getline(cin, line);
		if (line.empty()) continue;
		if (line == "--help") {
			print_help();
			cout << "> ";
			continue;
		}
		try {
			str += line + "\r\n";
			reset(str.c_str());

			parser.read_token();

			if (token_type == QUOTE)
				cout << token << endl;
			else if (token_type == VARIABLE)
				token_variable();
			else if (fun.is_fun(token)) {
				// function_act
			}
			else if (cmd.is_cmd(token, tok)) {
				if (cmd.is_return_cmd(tok)) {
					parser.putback_token();
					token_expression();
				}
				else cmd.execute(tok);
			}
			else {
				parser.putback_token();
				token_expression();
			}
		}
		catch (Exception& e) {
			if (e.type() != EXTRA_BRACKET || brackets.empty()) {
				std::cerr << e.what() << endl;
				brackets.clear();
			}
			else {
				print_tabs();
				continue;
			}
		}
		catch (...) {
			cout << "Something went wrong.\n"
				<< "Run-time error.\n";
			brackets.clear();
		}
		str.clear();
		cout << "> ";
	}
}


void Scanner::run(const char* filename) {
	std::string code;
	try {
		read_file(filename, code);

		reset(code.c_str());

		exec.eval(program);
	}
	catch (Exception& e) {
		std::cerr << e.what() << std::endl;
		brackets.clear();
	}
	catch (...) {
		std::cerr << "Something went wrong.\n"
			<< "Run-time error.\n";
		brackets.clear();
	}
	std::cin.get();
}


void Scanner::reset(const char* _str) const {
	program = prog_start = const_cast<char*>(_str);
	token_type = tok = 0;
	brackets.clear();
}

void Scanner::print_help() const {
	// TODO
	// rewrite instruction

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


void Scanner::print_tabs() const {
	size_t tab_max = brackets.is_brace(brackets.back()) ?
		brackets.braces_size() : brackets.size() - brackets.braces_size();

	for (size_t i = 0; i < tab_max; i++) std::cout << '\t';
}


void Scanner::token_variable() const {
	register char* temp = program - token.length();

	parser.read_token();
	parser.putback_token();
	program = temp;

	if (parser.is_expression(token[0]) || parser.is_end()) // %
		token_expression();
	else if (token[0] == '=')
		do {
			exec.assign_variable();
			//parser.read_token();
		} while (!parser.is_end());
	else throw Exception(INVALID_SYNTAX);
}


void Scanner::token_expression() const {
	register double res = exec.compute_exp();
	parser.read_token();

	if (!parser.is_end()) throw Exception(INVALID_SYNTAX);

	std::cout << res << std::endl;
}


void Scanner::read_file(const char* filename, std::string& code) const {
	std::ifstream fin(filename, std::ios::in | std::ios::binary);

	if (fin.is_open()) 
		code.assign((std::istreambuf_iterator<char>(fin)),
			std::istreambuf_iterator<char>());
	else throw Exception(NOT_OPEN_FILE);
}
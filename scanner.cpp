#include "scanner.h"



Scanner::Scanner() {}

Scanner::~Scanner() {}


void Scanner::run(const char* filename) {
	std::string code;
	try {
		read_file(filename, code);
		reset(code.c_str());

		exec.eval();
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
	std::cout << std::flush;
	std::cin.get();
}


void Scanner::reset(const char* _str) const {
	program = prog_start = const_cast<char*>(_str);
	token_type = tok = 0;
	brackets.clear();
	var.clear();
	fun.clear();
	//cmd.clear();
}

void Scanner::read_file(const char* filename, std::string& code) const {
	std::ifstream fin(filename, std::ios::in | std::ios::binary);

	if (fin.is_open()) 
		code.assign((std::istreambuf_iterator<char>(fin)),
			std::istreambuf_iterator<char>());
	else throw Exception(NOT_OPEN_FILE);
}
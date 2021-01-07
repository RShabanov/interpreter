#include <iostream>
#include <memory>
#include "Parser.h"
#include "init.h"
using namespace std;



int main() {
	// "5 - 3 + (3 - 5)"
	// "let var = 5, var2 = 2\r\nprint(var, var2)"
	//string str = "let var = 5, var2 = 2\r\nprint(var, var2, var3)";
	//prog_start = program = &str[0];
	////Parser parser;

	//double res = 0;
	//while (true) {
	//	try {
	//		parser.parse(res);
	//		continue;
	//		break;
	//		//cout << res << endl;
	//	}
	//	catch (ParserException& e) {
	//		if (e.type() != EXTRA_BRACKET || parentheses.empty()) {
	//			cout << e.what() << endl;
	//			break;
	//		}
	//		string temp;
	//		getline(cin, temp);
	//		str += temp;
	//		program = prog_start = &str[0];
	//		token_type = tok = 0;
	//		parentheses.clear();
	//	}
	//	catch (VariableException& e) {
	//		cout << e.what() << endl;
	//		break;
	//	}
	//}

	// отсутсвует поддержка любого языка, кроме English

	/*Init init_program;
	init_program.run();*/


	unique_ptr<string> films[5] = {
		unique_ptr<string>(new string("first")),
		unique_ptr<string>(new string("second")),
		unique_ptr<string>(new string("third")),
		unique_ptr<string>(new string("fourth")),
		unique_ptr<string>(new string("fifth")),
	};

	unique_ptr<string> winner;
	winner = films[2];

	cout << "Films:\n";
	for (int i = 0; i < 5; i++)
		cout << *films[i] << endl;
	cout << "Winner: " << *winner << endl;


	cin.get();
	return 0;
}
#include <iostream>
#include "Parser.h"
#include "scanner.h"



int main(int argc, char* argv[]) {
	using namespace std;

	Scanner scanner;
	scanner.run("./tests/math/log_1.txt");
	//scanner.run();

	/*program = const_cast<char*>(
		"((5 > 2) * ((3 > 7)) + (8 < 0)) * 2 == 0"
		);

	double res;
	parser.parse(res);

	cout << res << endl;
	cin.get();*/

	/*if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			cout << "--------------------------" << endl;
			cout << argv[i] << endl << endl;
			scanner.run(argv[i]);
		}
	}
	else {
		in = &cin;
		scanner.run();
	}*/

	return 0;
}
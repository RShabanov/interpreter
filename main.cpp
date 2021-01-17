#include <iostream>
#include "Parser.h"
#include "scanner.h"



int main(int argc, char* argv[]) {
	using namespace std;

	Scanner scanner;
	scanner.run("./tests/test_if_1.txt");

	//scanner.run();

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
#include <iostream>
#include <memory>
#include "Parser.h"
#include "scanner.h"


void foo(std::shared_ptr<std::string>& ptr) {
	using namespace std;

	shared_ptr<string> temp = make_shared<string>("my_string created in foo");
	cout << "foo" << endl;
	cout << temp << endl;

	ptr = temp;
}


int main(int argc, char* argv[]) {
	using namespace std;

	/*Scanner scanner;
	scanner.run("./tests/test_fun_6.txt");*/
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

	shared_ptr<string> ptr;
	foo(ptr);

	cout << "main after foo" << endl;
	cout << ptr << endl;

	system("pause");

	return 0;
}
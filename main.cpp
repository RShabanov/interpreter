#include <iostream>
#include "Parser.h"
#include "scanner.h"



void run_all_tests(Scanner&);


int main(int argc, char* argv[]) {
	using namespace std;

	Scanner scanner;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			cout << "--------------------------" << endl;
			cout << argv[i] << endl << endl;
			scanner.run(argv[i]);
		}
	}
	else run_all_tests(scanner);

	return 0;
}


void run_all_tests(Scanner& scanner) {
	// since my MVS doesn't support C++17
	// I do this

	constexpr size_t DIR_C = 5;
	std::string filename;

	struct {
		std::string dir;
		size_t files_cnt;
	} dirs[DIR_C] = {
		"fun", 12,
		"if-else", 11,
		"loop", 2,
		"math", 4,
		"other", 4
	};

	for (size_t i = 0; i < DIR_C; i++)
		for (size_t file_n = 0; file_n < dirs[i].files_cnt; file_n++) {
			filename = "./tests/" + dirs[i].dir + "/" +
				dirs[i].dir + "_" + std::to_string(file_n) + ".txt";

			std::cout << "Test of " << dirs[i].dir << std::endl
				<< "test # " << file_n << std::endl;

			scanner.run(filename.c_str());
			system("cls");
		}

	std::cout << "That's all" << std::endl;
	std::cin.get();
}
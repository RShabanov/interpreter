#include <iostream>
#include "Parser.h"
using namespace std;



int main() {

	string str = "5+(4-3)*2";

	Parser parser(&str[0]);

	double res = 0;
	parser.parse(res);
	cin.get();
	return 0;
}

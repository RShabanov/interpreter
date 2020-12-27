#include <iostream>
#include <cstdlib>
#include "Parser.h"
using namespace std;


class Exception {
public:
	void msg() {
		cout << "Something went wrong!\n";
	}

	friend ostream& operator<<(ostream& out, const Exception& e) {
		out << "Something went wrong!\n";
		return out;
	}
};

void foo(int t) {
	cout << "Foo!\n";
	if (t == 2) throw Exception();
}

void coo(int t) {
	foo(t);
	cout << "Coo!\n";
}

void boo(int t) {
	coo(t);
	cout << "Boo!\n";
}


struct A
{
	int a;
};

int main() {

	/*for (int i = 0; i < 5; i++) {
		cout << "i = " << i << endl;
		try {
			boo(i);
		}
		catch (Exception& e) {
			cout << e;
			continue;
		}
		cout << "i = " << i << endl;
	}*/

	A a;
	a.a = 50;

	const A* ptra = &a;


	cin.get();
	return 0;
}
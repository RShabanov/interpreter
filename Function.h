#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Parser.h"


class Function {

	// fun function_name(let var1, var2, ...) {
	//		... function_body
	//		[return]
	// }

	struct Fun {
		std::shared_ptr<std::string> body;
		int argc = 0;
		Fun(std::string& _body, int _argc)
			: body(std::make_shared<std::string>(_body)),
			argc(_argc) {}
	};

	std::multimap<std::string, Fun> funs;

	int check_params();

	friend class FunFunctor;
public:
	Function();
	~Function();

	bool is_fun(const std::string&);

	void add_fun(const std::string& _name,
		std::string& _body, int _argc);
	void delete_fun(const std::string&);
};
extern Function fun;
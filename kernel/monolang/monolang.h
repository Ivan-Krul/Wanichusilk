#pragma once
#include "../../abstractes/loader.h"
#include "token.h"
#include "flag.h"
class monolang : public loader
{
protected:
	mnlg::state _state;
	kernel_hardware _hardware;

	// set delay with printing
	void _cmdAwait();
	// manipulate with flag and set it
	void _cmdFlag();
	// is_end of the interpretation
	void _cmdEnd();
	// go to the line to string
	void _cmdGoto();
	// make monolog and give into string
	void _cmdLog();
	// make monochoise and give into class
	void _cmdChoose();
	// retarget interpretation in another file
	void _cmdFile();
	// create new definition of flag
	void _cmdGet();
	// mark action on name
	void _cmdAct();
	// operator if
	void _cmdIf();
	
public:
	void start(std::string dir_);
	void start(mnlg::state state_);
	void step();

	mnlg::access access();

	template<typename T>
	T flag();
	template<typename T>
	void flag(T thing_);

	template<typename T>
	T var(std::string var_);
	template<typename T>
	void var(T thing_, std::string var_);
	void new_var(std::string new_name_);

	monochoice choose();
	void choice(monochoice& choice_);

	std::string act(std::string who_);

	std::string log();
	std::string errorHandler();

	int delay();




	void load(std::string dir_, size_t line_) {}
	void parse(std::string raw_) {}
};
#include "monolang.cpp"
#include "method.cpp"
#pragma once
#include "../../abstractes/loader.h"
#include "token.h"
#include "flag.h"
#include <iostream>
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
	// without turning back, if it's have a format of interpretation
	void _cmdFile();

	void _stop();
public:
	void start(std::string dir_);
	void start(mnlg::state state_);
	void step();

	bool is_flag();
	template<typename T>
	T flag();

	bool is_log();
	std::string log();

	bool is_choose();
	monochoice choose();
	void choice(monochoice& choice_);

	std::string logger();
	bool is_aborted();
	std::string errorHandler();

	int delay();

	bool is_end();

	void load(std::string dir_, size_t line_) {}
	void parse(std::string raw_) {}

};
#include "monolang.cpp"
#include "method.cpp"
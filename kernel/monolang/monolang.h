#pragma once
#include "../../abstractes/loader.h"
#include "token.h"
class monolang : public loader
{
protected:
	state _state = { 0 };
	kernel_hardware _hardware;

	// set delay with printing
	void _cmdAwait();
	// manipulate with flag and set it
	void _cmdFlag();
	void _declairFlag(std::string type_);
	void _assignFlag();
	void _deleteFlag();
	// end of the interpretation
	void _cmdEnd();
	// go to the line to string
	void _cmdGoto();
	// make monolog and give into string
	// GOTO: delete monolog class
	void _cmdLog();
	// make monochoise and give into class
	// GOTO: shrink monochoise class
	void _cmdChoose();
	// retarget interpretation in another file
	// without turning back, if it's have a format of interpretation
	void _cmdFile();

public:
	void create(std::string dir_);

	void start();
	void step();

	bool take_flag();
	stateTypeFlag typedata_flag();
	void* get_resp();

	bool take_log();
	std::string get_log();

	void load(std::string dir_, size_t line_) {}
	void parse(std::string raw_) {}

};
#include "monolang.cpp"
#include "method.cpp"
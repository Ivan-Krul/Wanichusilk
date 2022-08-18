#pragma once
#include "../../abstractes/loader.h"
#include "token.h"
class monolang : public loader
{
protected:
	state _state;
	kernel_hardware _hardware;

	// set delay with printing
	void _cmdAwait();
	// manipulate with flag and set it
	void _cmdFlag();
	void _declairFlag(std::string type_);
	void _assignFlag();
	// end of the interpretation
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
	void start(state state_);
	void step();

	bool is_flag();
	stateTypeFlag typedata_flag();
	void* get_flag();
	bool get_flag(bool);
	char get_flag(char);
	int get_flag(int);
	float get_flag(float);
	std::string get_flag(std::string);

	bool is_log();
	std::string get_log();

	bool is_choose();
	monochoice get_choose();
	void get_choice(monochoice& choice_);

	std::string log();
	bool aborted();
	std::string errorHandler();

	int delay();

	bool end();

	void load(std::string dir_, size_t line_) {}
	void parse(std::string raw_) {}

};
#include "monolang.cpp"
#include "method.cpp"
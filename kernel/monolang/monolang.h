#pragma once
#include "../../abstractes/loader.h"
#include "token.h"
class monolang : public loader
{
protected:
	const std::string formatForInterpretation = "mns";
	const char breakpoint = '|';

	std::string _dir = "[directory]."+formatForInterpretation;
	state _state = { 0 };
	kernel_hardware _hardware;

	//set delay with printing (int)
	void _cmdAwait();
	//output flag (int/typename)
	void _cmdFlag();
	void _declairFlag(std::string type_);
	void _assignFlag();
	void _deleteFlag();
public:
	void create(std::string dir_);

	void start();
	void step();

	void load(std::string dir_, size_t line_);
	void parse(std::string raw_);

};
#include "monolang.cpp"
#include "method.cpp"
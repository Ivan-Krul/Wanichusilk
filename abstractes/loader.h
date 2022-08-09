#pragma once
#include "../kernel/hardware/hardware.h"
__interface Iloader
{
	void load(std::string dir_, size_t line_);
	void parse(std::string raw_);
};

class loader : public Iloader
{
protected:
	std::string _bakeParse(std::string& raw_, char breakpoint_);
public:
	void checkParse(std::string& raw_, std::string& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, char& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, short& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, int& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, long long& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, unsigned char& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, unsigned short& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, unsigned int& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, unsigned long long& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, float& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, double& thing_, char breakpoint_ = '|');
	void checkParse(std::string& raw_, long double& thing_, char breakpoint_ = '|');
};
#include "loader.cpp"
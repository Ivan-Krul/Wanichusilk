#include "loader.h"

std::string abstractes::loader::_bakeParse(std::string& raw_, char breakpoint_)
{
	size_t ind = 0;
	while(raw_[ind] != breakpoint_ && raw_.size() == ind)
		ind++;
	
	std::string buf = raw_.substr(0, ind);
	raw_ = raw_.substr(ind + 1);
	return buf;
}

void abstractes::loader::checkParse(std::string& raw_, std::string& thing_, char breakpoint_)
{
	thing_ = _bakeParse(raw_,breakpoint_);
}

void abstractes::loader::checkParse(std::string& raw_, bool& thing_, char breakpoint_)
{
	thing_ = _bakeParse(raw_, breakpoint_) == "true" ? true : false;
}

void abstractes::loader::checkParse(std::string& raw_, char& thing_, char breakpoint_)
{
	thing_ = _bakeParse(raw_, breakpoint_)[0];
}

void abstractes::loader::checkParse(std::string& raw_, short& thing_, char breakpoint_)
{
	thing_ = std::stoi(_bakeParse(raw_, breakpoint_));
}

void abstractes::loader::checkParse(std::string& raw_, int& thing_, char breakpoint_)
{
	thing_ = std::stoi(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, long long& thing_, char breakpoint_)
{
	thing_ = std::stoll(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, unsigned char& thing_, char breakpoint_)
{
	checkParse(raw_, (char&)thing_, breakpoint_);
}

inline void abstractes::loader::checkParse(std::string& raw_, unsigned short& thing_, char breakpoint_)
{
	thing_ = std::stoul(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, unsigned int& thing_, char breakpoint_)
{
	thing_ = std::stoul(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, unsigned long long& thing_, char breakpoint_)
{
	thing_ = std::stoull(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, float& thing_, char breakpoint_)
{
	thing_ = std::stof(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, double& thing_, char breakpoint_)
{
	thing_ = std::stod(_bakeParse(raw_, breakpoint_));
}

inline void abstractes::loader::checkParse(std::string& raw_, long double& thing_, char breakpoint_)
{
	thing_ = std::stold(_bakeParse(raw_, breakpoint_));
}


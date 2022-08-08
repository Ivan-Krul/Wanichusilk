#pragma once
#include <string>
#include <sstream>
class rawstring
{
protected:
	std::string _raw;
public:
	rawstring(std::string raw_);
	template <typename T> void operator<< (T& thing_);
	template <typename T> void operator>>(T& thing_);
	std::string raw();
};

template<typename T>
inline void rawstring::operator<<(T& thing_)
{
	std::stringstream str;
	str << thing_;
	_raw += str.str() + '|';
}

template<typename T>
inline void rawstring::operator>>(T& thing_)
{
	std::stringstream buffer;
	size_t ind = 0;
	while(_raw[ind] != '|')
	{
		buffer << _raw[ind];
		
		ind++;
	}
	_raw = _raw.substr(ind + 1);
	buffer >> thing_;
	char threw;
	buffer >> threw;
}

inline rawstring::rawstring(std::string raw_)
{
	_raw = raw_;
}

std::string rawstring::raw()
{
	return _raw;
}

#pragma once
#include "../../abstractes/loader.h"
class monolang
{
protected:
	size_t _line;
	std::string _dir;
public:
	monolang() {}
	monolang(std::string dir_) {create(dir_) }
	void create(std::string dir_);
};

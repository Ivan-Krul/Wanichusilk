#pragma once
#include "../kernel/hardware/hardware.h"
#include "rawstring.h"
__interface Iloader
{
	void load(std::string dir_, size_t line_);
	void parse(rawstring raw);
};


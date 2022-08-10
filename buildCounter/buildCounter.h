#pragma once
#include "../kernel/hardware/hardware.h"
class buildCounter
{
	size_t _buildNumber = 0;
public:
	buildCounter();
	size_t get();
};
#include "buildCounter.cpp"
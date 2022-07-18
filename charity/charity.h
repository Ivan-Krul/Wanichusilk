#pragma once
#include "../define.h"
#include "../abstractes/name.h"

class charity : public name {
	float _prod = 1.0f;
	int _def = 1;
	int _dam = 0;

public:
	void create(std::string name, int def,int dam);

	std::string info();
};
#include "charity.cpp"
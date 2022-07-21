#pragma once
#include "../define.h"
#include "../abstractes/name.h"

class charity;

__interface Icharity {
	void create(std::string name, unsigned int def, int dam);

	float prod();
	float damage(charity& charit);

	std::string info();
};

class charity : public name, public Icharity {
protected:
	float _prod = 1.0f;
	unsigned int _def = 0;
	int _dam = 0;

public:
	charity() : name() {}
	charity(std::string name, unsigned int def, int dam) : name() { create(name, def, dam); }
	void create(std::string name, unsigned int def,int dam);

	float prod() { return _prod; }
	float damage(charity& charit);

	std::string info();


	~charity() = default;
};
#include "charity.cpp"
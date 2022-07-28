#pragma once
#include "../../abstractes/name.h"

class kernel_charity;

__interface Ikernel_charity {
	void create(std::string name, unsigned int def, int dam);

	float prod();
	float damage(kernel_charity& charit);

	std::string info();
};

class kernel_charity : public name, public Ikernel_charity {
protected:
	float _prod = 1.0f;
	unsigned int _def = 0;
	int _dam = 0;

public:
	kernel_charity() : name() {}
	kernel_charity(std::string name, unsigned int def, int dam) : name() { create(name, def, dam); }
	void create(std::string name, unsigned int def,int dam);

	float prod() { return _prod; }
	float damage(kernel_charity& charit);

	std::string info();

	~kernel_charity() = default;
};
#include "charity.cpp"
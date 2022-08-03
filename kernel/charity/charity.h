#pragma once
#include "../../abstractes/name.h"

class kernel_charity : public name {
protected:
	float _prod = 1.0f;
	unsigned int _def = 0;
	int _dam = 0;
	int _edibleHeal = 0;

	void _digestion();
public:
	kernel_charity() : name() {}
	kernel_charity(std::string name, unsigned int def, int dam) : name()
	{
		create(name, def, dam);
	}
	void create(std::string name, unsigned int def,int dam);

	float prod()
	{
		return _prod;
	}
	float damage(kernel_charity& charit);
	void heal(float prod_) 
	{ 
		_prod += prod_;
	}
	void eat(int benefit_) 
	{ 
		_edibleHeal = benefit_;
	}

	void tick();

	std::string info();

	~kernel_charity() = default;
};
#include "charity.cpp"
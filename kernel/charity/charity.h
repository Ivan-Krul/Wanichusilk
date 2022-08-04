#pragma once
#include "../../abstractes/name.h"

class kernel_charity : public name {
protected:
	float _prod = 1.0f;
	uint16_t _def = 0;
	int16_t _atk = 0;
	int16_t _edibleHeal = 0;

	void _digestion();
public:
	kernel_charity() {}
	kernel_charity(std::string name, uint16_t def, int16_t atk);
	void create(std::string name, uint16_t def, int16_t atk);

	float damage(kernel_charity& charit);

	uint16_t defense_point();
	int16_t attack_point();
	int16_t edible_heal();

	void heal(float prod_);
	void hurt(float prod_);
	void eat(int16_t benefit_);

	void tick();

	std::string info();

	~kernel_charity() {};
};
#include "charity.cpp"
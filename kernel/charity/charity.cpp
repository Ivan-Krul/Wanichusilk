#include "charity.h"

kernel::charity::charity(std::string name, uint16_t def, int16_t atk, std::string description_)
{
	create(name, def, atk, _description);
}

void kernel::charity::create(std::string name, uint16_t def, int16_t atk, std::string description_)
{
	_name = name;
	_def = def;
	_atk = atk;
	_description = description_;
}

void kernel::charity::_digestion()
{
	heal((float)_edibleHeal / (_def + 1));
	_edibleHeal /= 2;
}

void kernel::charity::eat(int16_t benefit_)
{
	_edibleHeal = benefit_;
}

void kernel::charity::load(std::string dir_, size_t line_)
{
	hardware hardware;
	hardware.read_to_buffer_ln(dir_, line_);
	parse(hardware.buffer());
}

void kernel::charity::parse(std::string raw_)
{
	checkParse(raw_, _name);
	checkParse(raw_, _prod);
	checkParse(raw_, _atk);
	checkParse(raw_, _def);
	checkParse(raw_, _edibleHeal);
	checkParse(raw_, _description);
}

void kernel::charity::tick()
{
	_digestion();
}

std::string kernel::charity::get_description()
{
	return _name + " (def: " + std::to_string(_def) + " atk: " + std::to_string(_atk) + ") - " + _description;
}

float kernel::charity::damage(charity& charit)
{
	float colidedam = float(_atk) * _prod;
	colidedam /= float(charit.defense_point() + 1);

	charit.hurt(colidedam);
	return colidedam;
}

uint16_t kernel::charity::defense_point()
{
	return _def;
}

int16_t kernel::charity::attack_point()
{
	return _atk;
}

int16_t kernel::charity::edible_heal()
{
	return _edibleHeal;
}

void kernel::charity::heal(float prod_)
{
	_prod += prod_;
}

void kernel::charity::hurt(float prod_)
{
	_prod -= prod_;
}

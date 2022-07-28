#include "charity.h"

void kernel_charity::create(std::string name, unsigned int def, int dam) {
	_name = name;
	_def = def;
	_dam = dam;
}

std::string kernel_charity::info() {
	return _name + " - def: " + std::to_string(_def) + " atk: " + std::to_string(_dam);
}

float kernel_charity::damage(kernel_charity& charit) {
	float colidedam = float(_dam) * _prod;
	colidedam /= float(charit._def+1);

	charit._prod -= colidedam;
	return colidedam;
}

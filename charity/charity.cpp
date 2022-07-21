#include "charity.h"

void charity::create(std::string name, unsigned int def, int dam) {
	_name = name;
	_def = def;
	_dam = dam;
}

std::string charity::info() {
	return _name + " - def: " + std::to_string(_def) + " dam: " + std::to_string(_dam);
}

float charity::damage(charity& charit) {
	float colidedam = float(_dam) * _prod;
	colidedam /= float(charit._def+1);

	charit._prod -= colidedam;
	return colidedam;
}

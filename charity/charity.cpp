#include "charity.h"

void charity::create(std::string name, int def, int dam) {
	_name = name;
	_def = def;
	_dam = dam;
}

std::string charity::info() {
	return _name + " - def: " + std::to_string(_def) + " dam: " + std::to_string(_dam);
}

#pragma once
#include "../../abstractes/item.h"

class edible_sandwich : public edible_item {
	const static int _cheal = 5;
public:
	edible_sandwich() : edible_item(name("sandwich"), "sandwich (hil " + std::to_string(_cheal) + ") - simple combination of a bread, salad, hamand souces, borrowed from Heavy", _cheal) {}
};

class edible_banana : public edible_item {
	const static int _cheal = 3;
public:
	edible_banana() : edible_item(name("banana"), "banana (hil " + std::to_string(_cheal) + ") - fruit for fitness, filled potassium", _cheal) {}
};

class armor_helmet : public armor_item {
	const static int _cequip = 4;
public:
	armor_helmet() : armor_item(name("helmet"), "helmet (def " + std::to_string(_cequip) + ") - very huge and very massive", _cequip) {}
};

class weapon_stick : public weapon_item {
	const static int _cattack = 2;
public:
	weapon_stick() : weapon_item(name("stick"), "stick (atk " + std::to_string(_cattack) + ") - fresh, from branch", _cattack) {}
};

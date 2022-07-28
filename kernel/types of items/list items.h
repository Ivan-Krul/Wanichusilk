#pragma once
#include "../../abstractes/item.h"

class sandwich : public edible_item {
	const static int _cheal = 5;
public:
	sandwich() : edible_item(name("sandwich"), "sandwich (heal " + std::to_string(_cheal) + ") - simple combination of a bread, salad, hamand souces, borrowed from Heavy", _cheal) {}
};

class banana : public edible_item {
	const static int _cheal = 3;
public:
	banana() : edible_item(name("banana"), "banana (heal " + std::to_string(_cheal) + ") - fruit for fitness, filled potassium", _cheal) {}
};

class helmet : public equip_item {
	const static int _cequip = 4;
public:
	helmet() : equip_item(name("helmet"), "helmet (def " + std::to_string(_cequip) + ") - very huge and very massive", _cequip) {}
};

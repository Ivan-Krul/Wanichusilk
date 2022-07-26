#pragma once
#include "../../abstractes/item.h"

class sandwich : public edible_item {
	const static int _cheal = 5;
	
public:
	sandwich() : edible_item(name("sandwich"), "sandwich (heal " + std::to_string(_cheal) + ") - simple combination of a bread, salad, hamand souces, borrowed from Heavy", _cheal) {}
};

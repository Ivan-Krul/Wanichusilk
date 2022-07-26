#pragma once
#include "../../abstractes/item.h"

class sandwich : public edible_item {
	sandwich() : useful_item("sandwich", "simple combination of a bread, salad, ham and souces, borrowed from Heavy", 5);
};

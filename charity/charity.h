#pragma once
#include "../kernel/charity/charity.h"
#include "../abstractes/item.h"

class charity : public kernel_charity {
	armor_item _armor[2];
	weapon_item _weapon;
	item _bag[8] = nothing_item()[8];
public:
	void equip(armor_item armor_);
	void equip(weapon_item weapon_);
};

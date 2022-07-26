#pragma once
#include "item.h"

class useful_item : public item {
protected:
	int _benefit = 0;
	useful_item(std::string name_, std::string description_, int benefit_);
public:
	virtual int use();

	virtual ~useful_item() : ~item() {}
};

int useful_item::use() {
	!_use;
	return _benefit;
}

useful_item::useful_item(std::string name_, std::string description_, int benefit_) {
	_name = name_;
	_description = description_;
	_benefit = benefit_;
}

class nothing_item : public item {
	virtual int use() { return 0; }

	virtual ~nothing_item : ~useful_item() {}
};

class edible_item : public useful_item {};

class equip_item : public useful_item {};

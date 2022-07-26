#pragma once
#include "item.h"

class useful_item : public item {
protected:
	int _benefit = 0;
public:
	useful_item(name name_, std::string description_, int benefit_);
	virtual int use();

	virtual ~useful_item() {}
};

int useful_item::use() {
	_use = !_use;
	return _benefit;
}

useful_item::useful_item(name name_, std::string description_, int benefit_) {
	_name = name_.get_name();
	_description = description_;
	_benefit = benefit_;
}

class nothing_item : public item {
	virtual int use() { return 0; }

	virtual ~nothing_item() {}
};

class edible_item : public useful_item {
public:
	edible_item(name name_, std::string description_, int benefit_) : useful_item(name_, description_, benefit_) {}
};

class equip_item : public useful_item {
public:
	equip_item(name name_, std::string description_, int benefit_) : useful_item(name_, description_, benefit_) {}
};

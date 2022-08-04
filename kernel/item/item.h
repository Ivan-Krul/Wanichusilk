#pragma once
#include "../../define.h"
#include "../../abstractes/name.h"
#include <sstream>

__interface Iitem
{
	std::string get_description();
};

enum class type
{
	nothing,
	edible,
	heal,
	armor,
	weapon
};

class item : public Iitem, public name
{
protected:
	std::string _description = "[description]";
	type _type = type::nothing;
	int _use = 0;
public:
	item(){}
	item(name name_, std::string description_, type type_, int use_) { create(name_, description_, type_, use_); }
	void create(name name_, std::string description_, type type_, int use_);

	std::string description();
	type type_c();
	int usage();

	virtual ~item() {}
};
#include "item.cpp"
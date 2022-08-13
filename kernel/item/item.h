#pragma once
#include "../../define.h"
#include "../../abstractes/name.h"
#include "../../abstractes/loader.h"
#include "../../abstractes/description.h"
#include <sstream>

enum class type : char
{
	nothing = 'n',
	edible = 'e',
	heal = 'h',
	armor = 'a',
	weapon = 'w'
};

class kernel_item : public name, public loader, public description
{
protected:
	type _type = type::nothing;
	int _use = 0;

public:
	kernel_item(){}
	kernel_item(name name_, std::string description_, type type_, int use_) { create(name_, description_, type_, use_); }
	void create(name name_, std::string description_, type type_, int use_);

	std::string get_description();
	type type_c();
	int usage();
	void load(std::string dir_, size_t line_);
	void parse(std::string raw_);

	virtual ~kernel_item() {}
};
#include "item.cpp"
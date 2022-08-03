#pragma once
#include "../define.h"
#include "name.h"

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
	char _type = 'n';
	int _use = 0;
public:
	std::string get_description();
	
	virtual ~item() {}
};

std::string item::get_description()
{
	std::stringstream desc = _name;
	
	return _description;
}

#include "item advenced.h"
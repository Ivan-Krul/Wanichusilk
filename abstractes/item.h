#pragma once
#include "../define.h"
#include "name.h"

__interface Iitem {
	std::string get_description();
};

class item : public Iitem, public Iname {
protected:
	std::string _description = "[description]";
	name _name;
public:
	std::string get_name();
	std::string get_description();
	
	virtual ~item() {};
};

std::string item::get_name() {
	return _name.get_name();
}
std::string item::get_description() {
	return _description;
}

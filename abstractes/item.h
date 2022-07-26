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
	bool _use = true;
public:
	std::string get_name();
	std::string get_description();

	virtual int use() { !_use; };
	bool is_use() { return _use; }
	
	virtual ~item() {}
};

std::string item::get_name() {
	return _name.get_name();
}
std::string item::get_description() {
	return _description;
}

#include "item advenced.h"
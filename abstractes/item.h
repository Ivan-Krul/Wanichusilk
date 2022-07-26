#pragma once
#include "../define.h"
#include "name.h"

__interface Iitem {
	std::string get_description();
};

class item : public Iitem, public name {
protected:
	std::string _description = "[description]";
	bool _use = true;
public:
	std::string get_description();

	virtual int use() { _use = !_use; return 0; };
	bool is_use() { return _use; }
	
	virtual ~item() {}
};

std::string item::get_description() {
	return _description;
}

#include "item advenced.h"
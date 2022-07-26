#pragma once
#include "../define.h"
#include <string>

__interface Iname {
	std::string get_name();
};

class name : public Iname {
protected:
	std::string _name = "[name]";
public:
	name() {}
	name(std::string name_) { _name = name_; }
	std::string get_name();

	virtual ~name() {};
};

std::string name::get_name() {
	return _name;
}

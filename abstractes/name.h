#pragma once
#include "../define.h"
#include <string>

__interface Iname {
	std::string name();
};

class name : Iname {
protected:
	std::string _name = "[name]";
public:
	std::string name();

	virtual ~name() = 0;
};

inline std::string name::name() {
	return _name;
}

#pragma once
#include <string>
__interface Idescription
{
	std::string get_description();
};

class description
{
protected:
	std::string _description = "[description]";
public:

};

#include "description.cpp"
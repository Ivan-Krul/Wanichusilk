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
};

#include "description.cpp"
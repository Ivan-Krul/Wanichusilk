#pragma once
#include <string>
namespace abstractes
{
	__interface Idescription
	{
		std::string get_description();
	};

	class description
	{
	protected:
		std::string _description = "[description]";
	};
};

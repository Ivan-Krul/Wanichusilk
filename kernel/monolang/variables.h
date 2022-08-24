#pragma once
#include <map>
#include <string>
#include "flag.h"
namespace mnlg
{
	class variables
	{
	protected:
		std::map<std::string, flag> _map;
	public:
		void insert(std::string name_, flag flag_);
		flag get(std::string name_);
	};
};

void mnlg::variables::insert(std::string name_, flag flag_)
{
	_map.insert(std::make_pair<std::string,flag>(name_, flag_));
}

inline mnlg::flag mnlg::variables::get(std::string name_)
{
	return (*_map.find(name_)).second;
}

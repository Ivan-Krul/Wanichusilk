#pragma once
#include <fstream>
#include "../../define.h"
#include "token.h"
namespace kernel
{
	class dialang
	{
	protected:
		dalg::state _state;
		std::string _getline();
		std::string _divcommand(std::string& str_);
	public:
		void start(std::string dir_);
		void copy_state(dalg::state state_);
		void step();
		dalg::state state();
		void test();
	};
#include "dialang.cpp"
};
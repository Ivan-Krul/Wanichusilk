#pragma once
#include <fstream>
#include "../../define.h"
#include "token.h"
namespace kernel
{
	bool isdigit(char num_);
	int ctodigit(char num_);
	
	class dialang
	{
	protected:
		dalg::state _state;
		std::string _getline();
		std::string _divcommand(std::string& str_);
		bool _command(std::string& str_);

		bool _Cdelay(std::string& line_);
	public:
		void start(std::string dir_);
		void copy_state(dalg::state state_);
		void step();
		dalg::state state();
		void test();
	};
#include "dialang.cpp"
#include "methodes.cpp"
};
#pragma once
#include <fstream>
#include "../../define.h"
#include "token.h"
namespace kernel
{
	bool isdigit(char num_);
	int ctodigit(char num_);
	bool isspace(char chr_);
	std::string divide(std::string& str_, char start_, char end_);

	class dialang
	{
	protected:
		dalg::state _state;
		std::string _getline();
		bool _command(std::string& str_);
		void _methodestest();

		// commands
		bool _Cend(std::string& line_);
		bool _Cdelay(std::string& line_);
		bool _Cwait(std::string& line_);
		bool _Cgoto(std::string& line_);
		bool _Cfile(std::string& line_);
		bool _Ctask(std::string& line_);
		bool _Cif(std::string& line_);
		bool _Ctext(std::string& line_);
		bool _Cchoose(std::string& line_);
		bool _Cact(std::string& line_);
	public:
		void start(std::string dir_);
		void copy_state(dalg::state state_);
		void step();
		dalg::state state();
		void test();
		void load_task(std::string task_, bool is_started_, bool is_completed_);
		void load_choose(dalg::choose choose_);
		dalg::act get_act();
		dalg::text get_text();
	};
#include "dialang.cpp"
#include "methodes.cpp"
};
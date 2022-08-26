#include "dialang.h"



std::string kernel::dialang::_getline()
{
	std::string str;
	std::ifstream ifs;
	ifs.open(_state.dir);
	assert((ifs.is_open()));
	unsigned short i = 0;
	while(i < _state.line)
	{
		std::getline(ifs, str);
		i++;
	}
	ifs.close();
	return str;
}

std::string kernel::dialang::_divcommand(std::string& str_)
{
	std::string cmd = "";
	int startcommand, lencommand;
	for(startcommand = 0; startcommand < str_.size(); startcommand++) if(str_[startcommand] == _state.char_startcommand) break;
	for(lencommand = 0; (startcommand + lencommand + 1) < str_.size(); lencommand++)
	{
		if(str_[startcommand + lencommand + 1] == _state.char_endcommand) break;
		cmd += str_[startcommand + lencommand + 1];
	}
	str_ = str_.substr(lencommand + 1);
	return cmd;
}

bool kernel::dialang::_command(std::string& str_)
{
	std::string cmd = _divcommand(str_);
	for(auto var : dalg::tokenlist)
	{
		if(var.regex == cmd)
		{
			if(var.name == "DELAY");
			else if(var.name == "GET");
			else if(var.name == "IF");
			else if(var.name == "TEXT");
			else if(var.name == "CHOOSE");
			else if(var.name == "SET");
			else if(var.name == "WAIT");
			else if(var.name == "END") _state.is_end = true;
			else if(var.name == "GOTO");
			else if(var.name == "FILE");
			else if(var.name == "NEXT") _state.line++;
			else if(var.name == "ACT");
			return true;
		}
	}
	return false;
}

void kernel::dialang::start(std::string dir_)
{
	_state.char_operator = '|';
	_state.char_string = '"';
	_state.char_startcommand = '[';
	_state.char_endcommand = ']';
	_state.char_space = ' ';
	_state.dir = dir_;
	_state.line = 1;
	_state.is_end = false;
	_state.is_started = true;
	_state.delay = 0;
	_state.wait = 0;
}

void kernel::dialang::copy_state(dalg::state state_)
{
	_state = state_;
}

void kernel::dialang::step()
{
	if(_state.is_end || !_state.is_started) return;
	std::string line = _getline();
	while(!line.empty())
	{
		if(!_command(line))
		{
			std::cerr << "Element is undefined in line " + std::to_string(_state.line) + " in file " + _state.dir << '\n';
			assert((false));
			return;
		}
	}
}

kernel::dalg::state kernel::dialang::state()
{
	return _state;
}

void kernel::dialang::test()
{
	//_getline()
	std::ofstream ofs("abc.test");
	ofs << "hello, first case\nand now, second";
	ofs.close();
	_state.dir = "abc.test";
	_state.line = 1;
	assert((_getline() == "hello, first case"));
	_state.line = 2;
	assert((_getline() == "and now, second"));
	std::clog << "TEST: _getline() OK\n";

	//_divcommand()
	std::string strtest = "[hello][hi]";
	assert(("hello" == _divcommand(strtest)));
	assert(("hi" == _divcommand(strtest)));
	std::clog << "TEST: _divcommand() OK\n";

	//_command()
	strtest = "[end][delay][wtf]";
	assert((_command(strtest)));
	assert((_command(strtest)));
	assert((!_command(strtest)));
	std::clog << "TEST: _command() OK\n";

	//_Cdelay()
	strtest = "  100     no";
	assert((_Cdelay(strtest)));
	assert((_state.delay == 100));
	assert((!_Cdelay(strtest)));
	std::clog << "TEST: _Cdelay() OK\n";
}

bool kernel::isdigit(char num_)
{
	return '0' <= num_ && num_ <= '9';
}

int kernel::ctodigit(char num_)
{
	return num_ - '0';
}

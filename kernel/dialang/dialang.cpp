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

bool kernel::dialang::_command(std::string& str_)
{
	std::string cmd = divide(str_,_state.char_startcommand, _state.char_endcommand);
	for(auto var : dalg::tokenlist)
	{
		if(var.regex == cmd)
		{
			if(var.name == "DELAY") _Cdelay(str_);
			else if(var.name == "IF");
			else if(var.name == "TEXT");
			else if(var.name == "CHOOSE");
			else if(var.name == "TASK");
			else if(var.name == "WAIT") _Cwait(str_);
			else if(var.name == "END") _Cend(str_);
			else if(var.name == "GOTO");
			else if(var.name == "FILE");
			else if(var.name == "ACT");
			return true;
		}
	}
	return false;
}

void kernel::dialang::start(std::string dir_)
{
	_state.char_string = '"';
	_state.char_startcommand = '[';
	_state.char_endcommand = ']';
	_state.dir = dir_;
	_state.line = 1;
	_state.is_end = false;
	_state.is_started = true;
	_state.delay = 0;
	_state.wait = 0;
	_state.choose_.is_chosen = false;
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

	//divide()
	std::string strtest = "[hello][hi]";
	assert(("hello" == divide(strtest, _state.char_startcommand, _state.char_endcommand)));
	assert(("hi" == divide(strtest, _state.char_startcommand, _state.char_endcommand)));
	std::clog << "TEST: divide() OK\n";

	//_command()
	strtest = "[end][delay][wtf]";
	assert((_command(strtest)));
	assert((_command(strtest)));
	assert((!_command(strtest)));
	std::clog << "TEST: _command() OK\n";

	// another methodes
	_methodestest();
}

void kernel::dialang::load_task(std::string task_, bool is_started_, bool is_completed_)
{
	_state.mission_stage.insert({ (const std::string)task_,{is_started_,is_completed_,false} });
}

void kernel::dialang::load_choose(dalg::choose choose_)
{
	_state.choose_ = choose_;
	if(!_state.choose_.is_chosen || _state.choose_.choice == -1) return;
	for(int i = 0; i < _state.choose_.links.size(); i++)
		if(i == _state.choose_.choice)
		{
			_state.line = _state.choose_.links[i].second;
			break;
		}
}

void kernel::dialang::_methodestest()
{
	//_Cdelay()
	std::string strtest = "  100   also no";
	assert((_Cdelay(strtest)));
	assert((_state.delay == 100));
	assert((!_Cdelay(strtest)));
	std::clog << "TEST: _Cdelay() OK\n";

	//_Cwait()
	strtest = "  500 no";
	assert((_Cwait(strtest)));
	assert((_state.wait == 500));
	assert((!_Cwait(strtest)));
	std::clog << "TEST: _Cwait() OK\n";
}

bool kernel::isdigit(char num_)
{
	return '0' <= num_ && num_ <= '9';
}

int kernel::ctodigit(char num_)
{
	return num_ - '0';
}

bool kernel::isspace(char chr_)
{
	return (chr_ == ' ') || (chr_ == '\t') || (chr_ == '\r');
}

std::string kernel::divide(std::string& str_, char start_, char end_)
{
	std::string scope = "";
	while(str_.empty())
	{ 
		if(str_[0] == start_) break;
		str_ = str_.substr(1);
	}
	if(str_.empty()) return "";
	str_ = str_.substr(1);
	if(str_.empty()) return "";
	int len = 0;
	for(len = 0; len < str_.size(); len++)
	{
		if(str_[len] == end_) break;
		scope += str_[len];
	}
	str_ = str_.substr(len);
	return scope;
}

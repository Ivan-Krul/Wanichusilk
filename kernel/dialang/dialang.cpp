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
			else if(var.name == "IF") _Cif(str_);
			else if(var.name == "TEXT") _Ctext(str_);
			else if(var.name == "CHOOSE") _Cchoose(str_);
			else if(var.name == "TASK") _Ctask(str_);
			else if(var.name == "WAIT") _Cwait(str_);
			else if(var.name == "END") _Cend(str_);
			else if(var.name == "GOTO") _Cgoto(str_);
			else if(var.name == "FILE") _Cfile(str_);
			else if(var.name == "ACT") _Cact(str_);
			_state.line++;
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
	if(_state.is_end || !_state.is_started || _state.out != dalg::type_out::interacted) return;
	std::string line = _getline();
	if(!_command(line))
	{
		std::cerr << "Element is undefined in line " + std::to_string(_state.line) + " in file " + _state.dir << '\n';
		assert((false));
		return;
	}
}

kernel::dalg::state kernel::dialang::state()
{	
	auto s = _state;
	_state.out = dalg::type_out::interacted;
	return s;
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
	std::string strtest = "[hello][hi][]";
	assert(("hello" == divide(strtest, _state.char_startcommand, _state.char_endcommand)));
	assert(("hi" == divide(strtest, _state.char_startcommand, _state.char_endcommand)));
	assert(("" == divide(strtest, _state.char_startcommand, _state.char_endcommand)));
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
	// _Cdelay()
	std::string strtest = "  100   also no";
	assert((_Cdelay(strtest)));
	assert((_state.delay == 100));
	assert((!_Cdelay(strtest)));
	std::clog << "TEST: _Cdelay() OK\n";

	// _Cwait()
	strtest = "  500 no";
	assert((_Cwait(strtest)));
	assert((_state.wait == 500));
	assert((!_Cwait(strtest)));
	std::clog << "TEST: _Cwait() OK\n";

	// _Cgoto()
	strtest = " 30 60";
	_Cgoto(strtest);
	assert((_state.line == 30));
	_Cgoto(strtest);
	assert((_state.line == 60));
	std::clog << "TEST: _Cgoto() OK\n";

	// _Cfile()
	strtest = "[what.1] 1";
	assert((_Cfile(strtest)));
	assert((_state.dir == "what.1"));
	assert((_state.line == 1));
	std::clog << "TEST: _Cfile() OK\n";

	// _Ctask()
	load_task("wow", false, false);
	strtest = "[get][wow]";
	assert((_Ctask(strtest)));
	assert((_state.mission_stage.find("wow")->second.is_hotkey));
	strtest = "[add][wow]";
	assert((_Ctask(strtest)));
	assert((_state.mission_stage.find("wow")->second.is_started));
	std::clog << "TEST: _Ctask() OK\n";
	
	// _Cif()
	strtest = "[wow] 20";
	assert((_Cif(strtest)));
	assert((_state.line == 1));
	_state.mission_stage.find("wow")->second.is_completed = true;
	strtest = "[wow] 20";
	assert((_Cif(strtest)));
	assert((_state.line == 20));
	std::clog << "TEST: _Cif() OK\n";

	// _Ctext()
	strtest = "[dude][no] \"Yes\"";
	assert((_Ctext(strtest)));
	assert((_state.text_.author == "dude"));
	assert((_state.text_.emotion == "no"));
	assert((_state.text_.txt == "Yes"));
	std::clog << "TEST: _Ctext() OK\n";

	// _Cchoose()
	strtest = "[another dude] \"Wow\" 10 \"not Wow\" 20";
	assert((_Cchoose(strtest)));
	assert((_state.choose_.author == "another dude"));
	assert((_state.choose_.links[0].first == "Wow"));
	assert((_state.choose_.links[0].second == 10));
	assert((_state.choose_.links[1].first == "not Wow"));
	assert((_state.choose_.links[1].second == 20));
	std::clog << "TEST: _Cchoose() OK\n";

	// _Cact()
	strtest = "[abcd]";
	assert((_Cact(strtest)));
	assert((!_state.act_.is_null));
	strtest = "[null]";
	assert((_Cact(strtest)));
	assert((_state.act_.is_null));
	std::clog << "TEST: _Cact() OK\n";
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
	//clean all chars while start isn't appear
	std::string scope = "";
	while(!str_.empty())
	{ 
		if(str_[0] == start_) break;
		str_.erase(str_.begin());
	}
	if(str_.empty()) return "";
	//delete start symbol
	str_.erase(str_.begin());
	if(str_.empty()) return "";
	//copy text, while isn't end symbol
	int len = 0;
	for(len; len < str_.size(); len++)
	{
		if(str_[len] == end_) break;
		scope += str_[len];
	}
	//delete name and endchar if it could
	if(len == 0) return "";
	str_ = str_.substr(len + (str_.size() != len));
	return scope;
}

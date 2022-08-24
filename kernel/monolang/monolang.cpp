#include "monolang.h"

void monolang::start(std::string dir_)
{
	_state._line = 1;
	_state._await = 100;

	_state._flag = mnlg::flag();
	_state._errorHandler = MNLG_STABLE;

	_state._access = { 0 };

	_state.breakpoint = ' ';
	_state.formatForInterpretation = "mns";
	_state._dir = dir_;

	if(_hardware.is_open(dir_)) return;

	_state._access._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + dir_;
}

void monolang::start(mnlg::state state_)
{

	_state = state_;
	std::clog << "start\n";
	if(_hardware.is_open(_state._dir)) return;

	_state._access._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + _state._dir;
}

void monolang::step()
{
	if(_state._access._isAborted || _state._access._isEnd) return;
	_hardware.read_to_buffer_ln(_state._dir, _state._line);
	std::string string;
	checkParse(_hardware.buffer(), string, _state.breakpoint);

	for(auto iter : mnlg::listToken)
	{
		if(string == iter.regex)
		{

			if(iter.name == "AWAIT")  _cmdAwait();
			else if(iter.name == "CHOOSE") _cmdChoose();
			else if(iter.name == "END") _cmdEnd();
			else if(iter.name == "FILE") _cmdFile();
			else if(iter.name == "FLAG") _cmdFlag();
			else if(iter.name == "GOTO") _cmdGoto();
			else if(iter.name == "LOG") _cmdLog();
			else if(iter.name == "GET") _cmdGet();
			else if(iter.name == "ACT") _cmdAct();
			else if(iter.name == "IF") _cmdIf();

			_state._line++;
			return;
		}
	}
	_state._access._isAborted = true;
	_state._errorHandler = "[unstable] -> in " + std::to_string(_state._line) + " line is undefined char in file: "+_state._dir;
}

mnlg::access monolang::access()
{
	return _state._access;
}

template<typename T>
T monolang::flag()
{
	if(typeid(T).name() == "bool")
		return _state._flag.fbool();
	else if(typeid(T).name() == "char")
		return _state._flag.fchar();
	else if(typeid(T).name() == "int")
		return _state._flag.fint();
	else if(typeid(T).name() == "float")
		return _state._flag.ffloat();
	else if(typeid(T).name() == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		return _state._flag.fstring();
	else return T();
}

template<typename T>
void monolang::flag(T thing_)
{
	_state._access._isFlag = false;
	if(typeid(T).name() == "bool")
		_state._flag.fbool(thing_);
	else if(typeid(T).name() == "char")
		_state._flag.fchar(thing_);
	else if(typeid(T).name() == "int")
		_state._flag.fint(thing_);
	else if(typeid(T).name() == "float")
		_state._flag.ffloat(thing_);
	else if(typeid(T).name() == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		_state._flag.fstring(thing_);
}

template<typename T>
T monolang::var(std::string var_)
{
	if(typeid(T).name() == "bool")
		return _state._var.get(var_).fbool();
	else if(typeid(T).name() == "char")
		return _state._var.get(var_).fchar();
	else if(typeid(T).name() == "int")
		return _state._var.get(var_).fint();
	else if(typeid(T).name() == "float")
		return _state._var.get(var_).ffloat();
	else if(typeid(T).name() == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		return _state._var.get(var_).fstring();
	else return 0;
}

template<typename T>
void monolang::var(T thing_, std::string var_)
{
	_state._access._isVar = false;
	if(typeid(T).name() == "bool")
		_state._var.get(var_).fbool(thing_);
	else if(typeid(T).name() == "char")
		_state._var.get(var_).fchar(thing_);
	else if(typeid(T).name() == "int")
		_state._var.get(var_).fint(thing_);
	else if(typeid(T).name() == "float")
		_state._var.get(var_).ffloat(thing_);
	else if(typeid(T).name() == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >")
		_state._var.get(var_).fstring(thing_);
}

void monolang::new_var(std::string new_name_)
{
	_state._var.insert(new_name_, mnlg::flag());
}

std::string monolang::log()
{
	_state._access._isLog = false;
	return _state._log;
}

monochoice monolang::choose()
{
	return _state._choose;
}

void monolang::choice(monochoice& choice_)
{
	_state._access._isLog = true;
	if(choice_.is_assign()) _state._line = choice_.delta();
	else _state._line += choice_.delta();
}

std::string monolang::errorHandler()
{
	return _state._errorHandler;
}

std::string monolang::act(std::string who_)
{
	for(auto iter : _state._act)
	{
		if(iter.first == who_) return iter.second;
	}
	_state._access._isAborted = true;
	_state._errorHandler = "[unstable] -> name in list with acts didn't found: " + who_;
	return std::string();
}

int monolang::delay()
{
	return _state._await;
}


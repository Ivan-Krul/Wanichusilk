#include "monolang.h"

void monolang::start(std::string dir_)
{
	_state._line = 1;
	_state._await = 100;

	_state._flag = mnlg::flag();
	_state._errorHandler = MNLG_STABLE;

	_state._isAborted = false;
	_state._isEnd = false;

	_state.breakpoint = ' ';
	_state.formatForInterpretation = "mns";
	_state._dir = dir_;

	if(_hardware.is_open(dir_)) return;

	_state._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + dir_;
}

void monolang::start(mnlg::state state_)
{

	_state = state_;
	std::clog << "start\n";
	if(_hardware.is_open(_state._dir)) return;

	_state._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + _state._dir;
}

void monolang::step()
{
	if(_state._isAborted || _state._isEnd) return;
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

			_state._line++;
			return;
		}
	}
	_state._isAborted = true;
	_state._errorHandler = "[unstable] -> in " + std::to_string(_state._line) + " line is undefined char in file: "+_state._dir;
}

template<typename T>
T monolang::flag()
{
	_state._needTakeFlag = false;
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
	else return 0;
}

monochoice monolang::choose()
{
	return _state._choose;
}

void monolang::choice(monochoice& choice_)
{
	if(choice_.is_assign()) _state._line = choice_.delta();
	else _state._line += choice_.delta();
}

bool monolang::is_aborted()
{
	return _state._isAborted;
}

std::string monolang::errorHandler()
{
	return _state._errorHandler;
}

int monolang::delay()
{
	return _state._await;
}

bool monolang::is_end()
{
	return _state._isEnd;
}


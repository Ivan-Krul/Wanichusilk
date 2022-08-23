#include "monolang.h"

void monolang::_stop()
{
	if(_state._isEnd)
	{
		_state._logger = "[end]";
	}
	else if(_state._isAborted)
	{
		_state._logger = "[abort]";
	}
	else if(_state._needTakeFlag)
	{
		_state._logger = "[take flag]";
	}
	else if(_state._needTakeChoose)
	{
		_state._logger = "[take choose]";
	}
	else
	{
		_state._logger = "[take log]";
	}
}

void monolang::start(std::string dir_)
{
	_state._line = 1;
	_state._await = 100;

	_state._flag = mnlg::flag();
	_state._errorHandler = MNLG_STABLE;

	_state._needTakeFlag = false;
	_state._needTakeLog = false;
	_state._needTakeChoose = false;
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
	if(_state._isAborted || _state._isEnd || _state._needTakeChoose || _state._needTakeFlag || _state._needTakeLog)
	{
		_stop();
		std::clog << "monolang -> abort\n";
		std::clog << _state._logger << '\n' << _state._errorHandler << std::endl;
		return;
	}
	std::clog << "step\n";
	_hardware.read_to_buffer_ln(_state._dir, _state._line);
	std::string string;
	std::clog << string << std::endl;
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

bool monolang::is_flag()
{
	return _state._needTakeFlag;
}

template<typename T>
T monolang::flag()
{
	_state._needTakeFlag = false;
	if(typeid(T).name() == "bool" && _state._typeFlag == typeflag::BOOL)
		return *(bool*)(_state._flag);
	else if(typeid(T).name() == "char" && _state._typeFlag == typeflag::CHAR)
		return *(char*)(_state._flag);
	else if(typeid(T).name() == "int" && _state._typeFlag == typeflag::INT)
		return *(int*)(_state._flag);
	else if(typeid(T).name() == "float" && _state._typeFlag == typeflag::FLOAT)
		return *(float*)(_state._flag);
	else if(typeid(T).name() == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >" && _state._typeFlag == typeflag::STRING)
		return *(std::string*)(_state._flag);
	else return 0;
}

bool monolang::is_log()
{
	return _state._needTakeLog;
}

std::string monolang::log()
{
	_state._needTakeLog = false;
	return _state._log;
}

bool monolang::is_choose()
{
	return _state._needTakeChoose;
}

monochoice monolang::choose()
{
	return _state._choose;
}

void monolang::choice(monochoice& choice_)
{
	_state._needTakeChoose = false;
	if(choice_.is_assign()) _state._line = choice_.delta();
	else _state._line += choice_.delta();
}

std::string monolang::logger()
{
	return _state._logger;
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


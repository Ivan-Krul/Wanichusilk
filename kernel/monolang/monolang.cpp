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

	_state._typeFlag = stateTypeFlag::UNDEFINED;
	_state._errorHandler = MNLG_STABLE;

	_state._needTakeFlag = false;
	_state._needTakeLog = false;
	_state._needTakeChoose = false;
	_state._isAborted = false;
	_state._isEnd = false;

	_state.breakpoint = '|';
	_state.formatForInterpretation = "mns";
	_state._dir = dir_;

	if(_hardware.is_open(dir_)) return;

	_state._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + dir_;
}

void monolang::start(state state_)
{

	_state = state_;
	if(_hardware.is_open(_state._dir)) return;

	_state._isAborted = true;
	_state._errorHandler = "[unstable] -> file isn't open: " + _state._dir;
}

void monolang::step()
{
	if(_state._isAborted || _state._isEnd || _state._needTakeChoose || _state._needTakeFlag || _state._needTakeLog)
	{
		_stop();
		return;
	}
	_hardware.read_to_buffer_ln(_state._dir, _state._line);
	std::string string;
	checkParse(_hardware.buffer(), string, _state.breakpoint);
	
	for(auto iter : listToken)
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

stateTypeFlag monolang::typedata_flag()
{
	return _state._typeFlag;
}

void* monolang::get_flag()
{
	_state._needTakeFlag = false;
	return _state._flag;
}

bool monolang::get_flag(bool)
{
	if(_state._typeFlag != stateTypeFlag::BOOL)
	{
		_state._isAborted = true;
		_state._errorHandler = "[unstable] -> wrong type of variable";
		return NULL;
	}

	bool* var = (bool*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

char monolang::get_flag(char)
{
	if(_state._typeFlag != stateTypeFlag::CHAR)
	{
		_state._isAborted = true;
		_state._errorHandler = "[unstable] -> wrong type of variable";
		return NULL;
	}
	char* var = (char*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

int monolang::get_flag(int)
{
	if(_state._typeFlag != stateTypeFlag::INT)
	{
		_state._isAborted = true;
		_state._errorHandler = "[unstable] -> wrong type of variable";
		return NULL;
	}
	int* var = (int*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

float monolang::get_flag(float)
{
	if(_state._typeFlag != stateTypeFlag::FLOAT)
	{
		_state._isAborted = true;
		_state._errorHandler = "[unstable] -> wrong type of variable";
		return NULL;
	}
	float* var = (float*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

std::string monolang::get_flag(std::string)
{
	if(_state._typeFlag != stateTypeFlag::STRING)
	{
		_state._isAborted = true;

		_state._errorHandler = "[unstable] -> wrong type of variable";
		return NULL;
	}
	std::string* var = (std::string*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

bool monolang::is_log()
{
	return _state._needTakeLog;
}

std::string monolang::get_log()
{
	_state._needTakeLog = false;
	return _state._log;
}

bool monolang::is_choose()
{
	return _state._needTakeChoose;
}

monochoice monolang::get_choose()
{
	return _state._choose;
}

void monolang::get_choice(monochoice& choice_)
{
	_state._needTakeChoose = false;
	if(choice_.is_assign()) _state._line = choice_.delta();
	else _state._line += choice_.delta();
}

std::string monolang::log()
{
	return _state._logger;
}

bool monolang::aborted()
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

bool monolang::end()
{
	return _state._isEnd;
}


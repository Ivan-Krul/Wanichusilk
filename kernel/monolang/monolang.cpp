#include "monolang.h"

void monolang::create(std::string dir_)
{
	_dir = dir_;
	assert(_hardware.is_open(dir_));
}

void monolang::start()
{
	_state._line = 1;
	_state._isEnd = 0;
	_state._typeFlag = stateTypeFlag::UNDEFINED;
	_state._needTakeFlag = false;
}

void monolang::step()
{
	if(_state._needTakeFlag || _state._isEnd) return;
	_hardware.read_to_buffer_ln(_dir, _state._line);
	std::string string;
	checkParse(_hardware.buffer(), string, breakpoint);
	
	for(auto iter : listToken)
	{
		if(string == iter.regex)
		{
			_state._currentToken = iter;
			return;
		}
	}
}

bool monolang::take_flag()
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
	bool* var = (bool*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

char monolang::get_flag(char)
{
	char* var = (char*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

int monolang::get_flag(int)
{
	int* var = (int*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

float monolang::get_flag(float)
{
	float* var = (float*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

std::string monolang::get_flag(std::string)
{
	std::string* var = (std::string*)_state._flag;
	_state._needTakeFlag = false;
	return *var;
}

bool monolang::take_log()
{
	return _state._needTakeLog;
}

std::string monolang::get_log()
{
	_state._needTakeLog = false;
	return _state._log;
}

bool monolang::take_choose()
{
	return _state._needTakeChoose;
}

monochoice monolang::get_choose()
{
	_state._needTakeChoose = false;
	return _state._choose;
}


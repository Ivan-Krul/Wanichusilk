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

void* monolang::get_resp()
{
	_state._needTakeFlag = false;
	return _state._flag;
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


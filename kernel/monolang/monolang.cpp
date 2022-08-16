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
}

void monolang::step()
{
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

void monolang::load(std::string dir_, size_t line_)
{
}

void monolang::parse(std::string raw_)
{
}

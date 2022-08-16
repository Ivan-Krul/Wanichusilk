#include "monolang.h"

void monolang::_cmdAwait()
{
	checkParse(_hardware.buffer(), _state._await, breakpoint);
}

void monolang::_declairFlag(std::string type_)
{
	if(type_ == "[bool]")
	{
		_state._flag = (void*)new bool;
		_state._typeFlag = stateTypeFlag::BOOL;
	}
	else if(type_ == "[char]")
	{
		_state._flag = (void*)new char;
		_state._typeFlag = stateTypeFlag::CHAR;
	}
	else if(type_ == "[int]")
	{
		_state._flag = (void*)new int;
		_state._typeFlag = stateTypeFlag::INT;
	}
	else if(type_ == "[float]")
	{
		_state._flag = (void*)new float;
		_state._typeFlag = stateTypeFlag::FLOAT;
	}
	else if(type_ == "[string]")
	{
		_state._flag = (void*)new std::string;
		_state._typeFlag = stateTypeFlag::STRING;
	}
}

void monolang::_assignFlag()
{
	switch(_state._typeFlag)
	{
		case stateTypeFlag::BOOL:
			bool* var = (bool*)_state._flag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::CHAR:
			char* var = (char*)_state._flag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::INT:
			int* var = (int*)_state._flag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::FLOAT:
			float* var = (float*)_state._flag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::STRING:
			std::string* var = (std::string*)_state._flag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		default:
			break;
	}
}

void monolang::_deleteFlag()
{
	switch(_state._typeFlag)
	{
		case stateTypeFlag::BOOL:
			bool* var = (bool*)_state._flag;
			delete var;
			break;
		case stateTypeFlag::CHAR:
			char* var = (char*)_state._flag;
			delete var;
			break;
		case stateTypeFlag::INT:
			int* var = (int*)_state._flag;
			delete var;
			break;
		case stateTypeFlag::FLOAT:
			float* var = (float*)_state._flag;
			delete var;
			break;
		case stateTypeFlag::STRING:
			std::string* var = (std::string*)_state._flag;
			delete var;
			break;
		default:
			break;
	}
}

void monolang::_cmdEnd()
{
	_state._isEnd = true;
	_deleteFlag();
}

void monolang::_cmdFlag()
{
	std::string checker = _hardware.buffer();
	char firstSym;
	checkParse(checker, firstSym, breakpoint);
	if(firstSym == '[')
	{
		std::string type;
		checkParse(_hardware.buffer(), type, breakpoint);
		for(auto iter : listToken)
		{
			if(type == iter.regex)
			{
				_declairFlag(type);
				return;
			}
		}
	}
	else
	{
		_assignFlag();
		_state._needTakeFlag = true;
	}
}

void monolang::_cmdGoto()
{
	std::string operato;
	int delta;
	checkParse(_hardware.buffer(), operato, breakpoint);
	checkParse(_hardware.buffer(), delta, breakpoint);

	if(operato == "[-]") _state._line -= delta;
	else if(operato == "[+]") _state._line += delta;
	else _state._line = delta;
}

void monolang::_cmdLog()
{
	std::string author;
	std::string text;

	checkParse(_hardware.buffer(), author, breakpoint);
	checkParse(_hardware.buffer(), text, breakpoint);

	if(author == "[action]") _state._log = "";
	else _state._log = author + ": ";
	_state._log += text;
																																																																																																						
	_state._needTakeLog = true;
}


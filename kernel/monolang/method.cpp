#include "monolang.h"

void monolang::_cmdAwait()
{
	checkParse(_hardware.buffer(), _state._await, breakpoint);
}

void monolang::_declairFlag(std::string type_)
{
	if(type_ == "[bool]")
	{
		_state._returnedFlag = (void*)new bool;
		_state._typeFlag = stateTypeFlag::BOOL;
	}
	else if(type_ == "[char]")
	{
		_state._returnedFlag = (void*)new char;
		_state._typeFlag = stateTypeFlag::CHAR;
	}
	else if(type_ == "[int]")
	{
		_state._returnedFlag = (void*)new int;
		_state._typeFlag = stateTypeFlag::INT;
	}
	else if(type_ == "[float]")
	{
		_state._returnedFlag = (void*)new float;
		_state._typeFlag = stateTypeFlag::FLOAT;
	}
	else if(type_ == "[string]")
	{
		_state._returnedFlag = (void*)new std::string;
		_state._typeFlag = stateTypeFlag::STRING;
	}
}

void monolang::_assignFlag()
{
	switch(_state._typeFlag)
	{
		case stateTypeFlag::BOOL:
			bool* var = (bool*)_state._returnedFlag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::CHAR:
			char* var = (char*)_state._returnedFlag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::INT:
			int* var = (int*)_state._returnedFlag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::FLOAT:
			float* var = (float*)_state._returnedFlag;
			checkParse(_hardware.buffer(), *var, breakpoint);
			break;
		case stateTypeFlag::STRING:
			std::string* var = (std::string*)_state._returnedFlag;
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
			bool* var = (bool*)_state._returnedFlag;
			delete var;
			break;
		case stateTypeFlag::CHAR:
			char* var = (char*)_state._returnedFlag;
			delete var;
			break;
		case stateTypeFlag::INT:
			int* var = (int*)_state._returnedFlag;
			delete var;
			break;
		case stateTypeFlag::FLOAT:
			float* var = (float*)_state._returnedFlag;
			delete var;
			break;
		case stateTypeFlag::STRING:
			std::string* var = (std::string*)_state._returnedFlag;
			delete var;
			break;
		default:
			break;
	}
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
	}
}

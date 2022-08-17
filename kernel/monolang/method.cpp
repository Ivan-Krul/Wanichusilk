#include "monolang.h"

void monolang::_cmdAwait()
{
	checkParse(_hardware.buffer(), _state._await, _state.breakpoint);
}

void monolang::_declairFlag(std::string type_)
{
	if(type_ == "[bool]")
	{
		_state._flag = malloc(sizeof(bool));
		_state._typeFlag = stateTypeFlag::BOOL;
	}
	else if(type_ == "[char]")
	{
		_state._flag = malloc(sizeof(char));
		_state._typeFlag = stateTypeFlag::CHAR;
	}
	else if(type_ == "[int]")
	{
		_state._flag = malloc(sizeof(int));
		_state._typeFlag = stateTypeFlag::INT;
	}
	else if(type_ == "[float]")
	{
		_state._flag = malloc(sizeof(float));
		_state._typeFlag = stateTypeFlag::FLOAT;
	}
	else if(type_ == "[string]")
	{
		_state._flag = malloc(sizeof(std::string));
		_state._typeFlag = stateTypeFlag::STRING;
	}
}

void monolang::_assignFlag()
{
	switch(_state._typeFlag)
	{
		case stateTypeFlag::BOOL:
			{
				bool* var = (bool*)_state._flag;
				checkParse(_hardware.buffer(), *var, _state.breakpoint);
			}
			break;
		case stateTypeFlag::CHAR:
			{
				char* var = (char*)_state._flag;
				checkParse(_hardware.buffer(), *var, _state.breakpoint);
			}
			break;
		case stateTypeFlag::INT:
			{
				int* var = (int*)_state._flag;
				checkParse(_hardware.buffer(), *var, _state.breakpoint);
			}
			break;
		case stateTypeFlag::FLOAT:
			{
				float* var = (float*)_state._flag;
				checkParse(_hardware.buffer(), *var, _state.breakpoint);
			}
			break;
		case stateTypeFlag::STRING:
			{
				std::string* var = (std::string*)_state._flag;
				checkParse(_hardware.buffer(), *var, _state.breakpoint);
			}
			break;
		default:
			break;
	}
}

void monolang::_cmdEnd()
{
	_state._isEnd = true;
	free(_state._flag);
}

void monolang::_cmdFlag()
{
	std::string checker = _hardware.buffer();
	char firstSym;
	checkParse(checker, firstSym, _state.breakpoint);
	if(firstSym == '[')
	{
		if(_state._typeFlag != stateTypeFlag::UNDEFINED) free(_state._flag);
		std::string type;
		checkParse(_hardware.buffer(), type, _state.breakpoint);
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
	checkParse(_hardware.buffer(), operato, _state.breakpoint);
	checkParse(_hardware.buffer(), delta, _state.breakpoint);

	if(operato == "[-]") _state._line -= delta;
	else if(operato == "[+]") _state._line += delta;
	else _state._line = delta;
}

void monolang::_cmdLog()
{
	std::string author;
	std::string text;

	checkParse(_hardware.buffer(), author, _state.breakpoint);
	checkParse(_hardware.buffer(), text, _state.breakpoint);

	if(author == "[action]") _state._log = "";
	else _state._log = author + ": ";
	_state._log += text;
	_state._needTakeLog = true;
}

void monolang::_cmdChoose()
{
	std::string author;
	std::vector <Variant> choice;
	size_t size;

	checkParse(_hardware.buffer(), author, _state.breakpoint);
	checkParse(_hardware.buffer(), size, _state.breakpoint);
	choice.resize(size);
	Variant pair;
	std::string oper;
	for(size_t i = 0; i < size; i++)
	{
		checkParse(_hardware.buffer(), pair._str, _state.breakpoint);
		checkParse(_hardware.buffer(), oper, _state.breakpoint);
		checkParse(_hardware.buffer(), pair._delta, _state.breakpoint);
		if(oper == "[+]")
		{
			pair._isAssign = false;
		}
		else if(oper == "[-]")
		{
			pair._delta *= -1;
			pair._isAssign = false;
		}
		else 
		{ 
			pair._isAssign = true;
		}

		choice[i] = pair;
	}
	_state._choose.create(author, choice);
	_state._needTakeChoose = true;
}

void monolang::_cmdFile()
{
	std::string dir;
	size_t line;
	checkParse(_hardware.buffer(), dir, _state.breakpoint);
	checkParse(_hardware.buffer(), line, _state.breakpoint);
	
	if(!_hardware.is_open(dir))
	{
		_state._isAborted = true;
		_state._errorHandler = "[unstable] -> file isn't open: " + dir;
		return;
	}

	std::string format = dir.substr(dir.size()-3);
	if(format == _state.formatForInterpretation)
	{
		_state._dir = dir;
		_state._line = line;
	}
	else
	{
		_hardware.read_to_buffer_ln(dir, line);
		_state._logger = _hardware.buffer();
	}
}


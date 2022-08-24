#include "monolang.h"

void monolang::_cmdAwait()
{
	checkParse(_hardware.buffer(), _state._await, _state.breakpoint);
}

void monolang::_cmdEnd()
{
	_state._isEnd = true;
	_state._flag.~flag();
}

void monolang::_cmdFlag()
{
	std::string checker;
	checkParse(_hardware.buffer(), checker, _state.breakpoint);
	
	_state._flag.from_string(checker);
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
		if(oper == "+")
		{
			pair._isAssign = false;
		}
		else if(oper == "-")
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
}

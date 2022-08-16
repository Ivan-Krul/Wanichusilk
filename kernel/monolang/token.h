#pragma once
#include <string>
#include <list>
struct token
{
	std::string name;
	std::string regex;
};

enum class stateTypeFlag : int
{
	UNDEFINED = 0b000,
	BOOL = 0b001,
	CHAR = 0b010,
	INT = 0b011,
	FLOAT = 0b100,
	STRING = 0b101
};
struct state
{
	std::string formatForInterpretation = "mns";
	std::string _dir = "[directory]." + formatForInterpretation;
	char breakpoint = '|';

	bool _needTakeFlag : 1;
	bool _needTakeLog : 1;
	bool _isEnd : 1;

	stateTypeFlag _typeFlag : 3;
	void* _flag;
	
	std::string _log;

	unsigned short _line;
	unsigned short _await;
};

std::list<token>listToken = {
	{"AWAIT","[await]"},
	{"FLAG","[flag]"},
	{"TYPE_INT","[int]"},
	{"TYPE_STRING","[string]"},
	{"TYPE_FLOAT","[float]"},
	{"TYPE_CHAR","[char]"},
	{"TYPE_BOOL","[bool]"},
	{"END","[end]"},
	{"GOTO","[goto]"},
	{"LOG","[log]"},
	{"CHOOSE","[choose]"},
	{"FILE","[file]"},
};

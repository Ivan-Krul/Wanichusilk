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
	unsigned short _line;
	stateTypeFlag _typeFlag : 3;
	void* _returnedFlag;
	int _isEnd : 1;
	token _currentToken;
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
	{"PLUS","[+]"},
	{"ASSIGN","[=]"},
	{"MINUS","[-]"},
	{"GOTO","[goto]"},
	{"LOG","[log]"},
	{"CHOOSE","[choose]"},
	{"FILE","[file]"},
};

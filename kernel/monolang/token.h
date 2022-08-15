#pragma once
#include <string>
#include <list>
struct token
{
	std::string name;
	std::string regex;
};

std::list<token>listToken = {
	{"AWAIT","[await]"},
	{"FLAG","[flag]"},
	{"SET","[set]"},
	{"TYPE_INT","[int]"},
	{"TYPE_STRING","[string]"},
	{"TYPE_CHAR","[char]"},
	{"TYPE_BOOL","[bool]"},
	{"END","[end]"},
	{"GOTO","[goto]"},
	{"PLUS","[+]"},
	{"ASSIGN","[=]"},
	{"MINUS","[-]"},
	{"GOTO","[goto]"},
	{"MONO*","[mono/*]"},
	{"LOG","[log]"},
	{"CHOOSE","[choose]"},
	{"FILE","[file]"},
};

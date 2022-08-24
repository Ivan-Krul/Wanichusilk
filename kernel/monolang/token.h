#pragma once
#include <string>
#include <list>
#include "../monochoice/monochoice.h"
#include "flag.h"
namespace mnlg
{
#define MNLG_STABLE "[stable]"
	struct token
	{
		std::string name;
		std::string regex;
	};

	struct state
	{
		std::string formatForInterpretation = "mns";
		std::string _dir = "[directory]." + formatForInterpretation;
		std::string _errorHandler = "[error]";
		char breakpoint = ' ';

		bool _isAborted : 1;
		bool _isEnd : 1;

		flag _flag;

		std::string _log;
		monochoice _choose;

		unsigned short _line;
		unsigned short _await;
	};

	std::list<token>listToken = {
		{"AWAIT","await"},
		{"FLAG","flag"},
		{"END","end"},
		{"GOTO","goto"},
		{"LOG","log"},
		{"CHOOSE","choose"},
		{"FILE","disk"},
		{"GET","get"},
		{"ACT","act"},
	};

	std::list<token>listTypeVariable = {
		{"TYPE_INT","int"},
		{"TYPE_STRING","string"},
		{"TYPE_FLOAT","float"},
		{"TYPE_CHAR","char"},
		{"TYPE_BOOL","bool"}
	};
}

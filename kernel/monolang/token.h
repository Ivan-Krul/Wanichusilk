#pragma once
#include <string>
#include <list>
#include "../monochoice/monochoice.h"
#include "flag.h"
#include "variables.h"
namespace mnlg
{
#define MNLG_STABLE "[stable]"
	struct token
	{
		std::string name;
		std::string regex;
	};

	struct access
	{
		bool _isFlag : 1;
		bool _isAct : 1;
		bool _isLog : 1;
		bool _isChoose : 1;
		bool _isVar : 1;
		bool _isAborted : 1;
		bool _isEnd : 1;
	};

	struct state
	{
		std::string formatForInterpretation = "mns";
		std::string _dir = "[directory]." + formatForInterpretation;
		std::string _errorHandler = "[error]";
		char breakpoint = ' ';

		access _access;

		flag _flag;
		std::list<std::pair<std::string, std::string>> _act;
		std::string _log;
		monochoice _choose;
		variables _var;

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
		{"IF","if"},
	};

	std::list<token>listTypeVariable = {
		{"TYPE_INT","int"},
		{"TYPE_STRING","string"},
		{"TYPE_FLOAT","float"},
		{"TYPE_CHAR","char"},
		{"TYPE_BOOL","bool"}
	};
}

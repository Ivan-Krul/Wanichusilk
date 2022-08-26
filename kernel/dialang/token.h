#pragma once
#include <string>
#include <list>
namespace kernel::dalg
{
	struct token
	{
		std::string name;
		std::string regex;
	};
	struct state
	{
		char char_operator = '|';
		char char_string = '"';
		char char_startcommand = '[';
		char char_endcommand = ']';
		char char_space = ' ';
		std::string dir;

		unsigned short line;

		bool is_started : 1;
		bool is_end : 1;
		int delay : 10;
		int wait : 14;
	};
	const std::list<token> tokenlist = {
		{"DELAY","delay"},
		{"GET","get"},
		{"IF","if"},
		{"TEXT","text"},
		{"CHOOSE","choose"},
		{"SET","set"},
		{"WAIT","wait"},
		{"END","end"},
		{"GOTO","goto"},
		{"FILE","file"}
	};
	const std::list<token> operatorlist = {
		{"NEXT_COMMAND","->"},
		{"NEXT_ELEMENT",","},
	};
};

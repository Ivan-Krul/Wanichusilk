#pragma once
#include <string>
#include <list>
#include <map>
namespace kernel::dalg
{
	struct token
	{
		std::string name;
		std::string regex;
	};
	struct flagtask
	{
		bool is_started : 3;
		bool is_completed : 2;
		bool is_hotkey : 3;
	};
	struct state
	{
		char char_operator = '|';
		char char_string = '"';
		char char_startcommand = '[';
		char char_endcommand = ']';
		std::string dir;

		unsigned short line;
		std::map<std::string, flagtask> mission_stage;

		bool is_started : 1;
		bool is_end : 1;
		int delay : 10;
		int wait : 14;
	};
	const std::list<token> tokenlist = {
		{"DELAY","delay"},//
		{"IF","if"},
		{"TEXT","text"},
		{"CHOOSE","choose"},
		{"TASK","task"},
		{"WAIT","wait"},//
		{"END","end"},//
		{"GOTO","goto"},//
		{"FILE","file"},//
		{"ACT","act"},
	};
	const std::list<token> operatorlist = {
		{"NEXT_COMMAND","->"},
		{"NEXT_ELEMENT",","},
	};
};

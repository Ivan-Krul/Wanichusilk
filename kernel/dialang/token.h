#pragma once
#include <string>
#include <list>
#include <vector>
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
	struct text
	{
		std::string author;
		std::string emotion;
		std::string text;
	};
	struct choose {
		std::string author;
		std::vector<std::pair<std::string, int>> links;
		int choice = -1;
		bool is_chosen = false;
	};
	struct act
	{
		std::string name_act;
	};
	struct state
	{
		char char_string = '"';
		char char_startcommand = '[';
		char char_endcommand = ']';
		std::string dir;

		unsigned short line;
		std::map<std::string, flagtask> mission_stage;
		text text_;
		choose choose_;
		act act_;

		bool is_started : 1;
		bool is_end : 1;
		int delay : 10;
		int wait : 14;
	};
	const std::list<token> tokenlist = {
		{"DELAY","delay"},//
		{"IF","if"},//
		{"TEXT","text"},
		{"CHOOSE","choose"},
		{"TASK","task"},//
		{"WAIT","wait"},//
		{"END","end"},//
		{"GOTO","goto"},//
		{"FILE","file"},//
		{"ACT","act"},
	};
};

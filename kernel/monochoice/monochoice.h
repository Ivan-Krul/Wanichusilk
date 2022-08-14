#pragma once
#include <vector>
#include "../../abstractes/loader.h"
#include "../../abstractes/name.h"
class monochoice : public name, public loader
{
protected:
	std::vector<std::pair<size_t, std::string>> _choice;
	size_t _numChoice = 0;
	bool _isChoosed = false;
public:
	monochoice() {}
	monochoice(std::string name_, std::vector<std::pair<size_t, std::string>>choice_) { create(name_, choice_); }
	void create(std::string name_, std::vector<std::pair<size_t, std::string>>choice_);

	void push_back(std::string new_choice_, size_t ind_);

	void switch_next();
	void switch_prev();
	size_t numChoice() { return _numChoice; }
	bool isChoosed() { return _isChoosed; }

	void choose();

	void load(std::string dir_, size_t line_);
	void parse(std::string raw_);

};
#include "monochoice.cpp"
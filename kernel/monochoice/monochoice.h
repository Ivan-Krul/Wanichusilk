#pragma once
#include <vector>
#include "../../abstractes/loader.h"
#include "../../abstractes/name.h"
class monochoice : public name
{
protected:
	std::vector<std::pair<int, std::string>> _choice;
	size_t _numChoice = 0;
	bool _isChoosed = false;
public:
	monochoice() {}
	monochoice(std::string name_, std::vector<std::pair<int, std::string>>choice_) { create(name_, choice_); }
	void create(std::string name_, std::vector<std::pair<int, std::string>>choice_);

	void switch_next();
	void switch_prev();
	size_t numChoice() { return _numChoice; }
	bool isChoosed() { return _isChoosed; }

	void choose();
};
#include "monochoice.cpp"
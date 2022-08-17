#pragma once
#include <vector>
#include "../../abstractes/loader.h"
#include "../../abstractes/name.h"
struct Variant
{
	int _delta;
	bool _isAssign;
	std::string _str;
};

class monochoice : public name
{
protected:
	std::vector<Variant> _choice;
	size_t _numChoice = 0;
	bool _isChoosed = false;
public:
	monochoice() {}
	monochoice(std::string name_, std::vector<Variant>choice_) { create(name_, choice_); }
	void create(std::string name_, std::vector<Variant>choice_);

	void switch_next();
	void switch_prev();
	size_t numChoice() { return _numChoice; }
	bool isChoosed() { return _isChoosed; }

	size_t size() { return _choice.size(); }
	std::string variant(size_t ind_);

	void choose(size_t what_);
	void choose();

	bool is_assign() { return _choice[_numChoice]._isAssign; }
	int delta() { return _choice[_numChoice]._delta; }
};
#include "monochoice.cpp"
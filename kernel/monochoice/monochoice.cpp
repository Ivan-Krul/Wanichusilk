#include "monochoice.h"

void monochoice::create(std::string name_, std::vector<Variant> choice_)
{
	_name = name_;
	_choice = choice_;
}

void monochoice::switch_next()
{
	if(!_isChoosed)
	{
		_numChoice++;
		_numChoice %= _choice.size();
	}
}

void monochoice::switch_prev()
{
	if(!_isChoosed)
	{
		_numChoice--;
		_numChoice %= _choice.size();
	}
}

std::string monochoice::variant(size_t ind_)
{
	ind_ %= _choice.size();
	return _choice[ind_]._str;
}

void monochoice::choose(size_t what_)
{
	what_ %= _choice.size();
	_numChoice = what_;
	_isChoosed = true;
}

void monochoice::choose()
{
	_isChoosed = true;
}


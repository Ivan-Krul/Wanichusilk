#include "monochoice.h"

void monochoice::create(std::string name_, std::vector<std::pair<int, std::string>> choice_)
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

void monochoice::choose()
{
	_isChoosed = true;
}


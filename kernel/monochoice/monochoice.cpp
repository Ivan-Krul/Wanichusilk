#include "monochoice.h"

void monochoice::create(std::string name_, std::vector<std::pair<size_t, std::string>> choice_)
{
	_name = name_;
	_choice = choice_;
}

void monochoice::push_back(std::string new_choice_,size_t ind_)
{
	_choice.push_back(std::pair<size_t, std::string>(ind_,new_choice_));
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

void monochoice::load(std::string dir_, size_t line_)
{
	kernel_hardware hardware;
	hardware.read_to_buffer_ln(dir_, line_);
	std::string raw = hardware.buffer();

	parse(raw);
}

void monochoice::parse(std::string raw_)
{

}

#include "dialang.h"

bool kernel::dialang::_Cdelay(std::string& line_)
{
	//delete space and knowing is input - number
	while(line_[0] == _state.char_space) line_ = line_.substr(1);
	if(!isdigit(line_[0])) return false;

	//parse, when digits is ended
	_state.delay = 0;
	while(isdigit(line_[0]))
	{
		_state.delay = _state.delay * 10 + ctodigit(line_[0]);
		line_ = line_.substr(1);
	}
	return true;
}

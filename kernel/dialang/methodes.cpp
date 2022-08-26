#include "dialang.h"

bool kernel::dialang::_Cend(std::string& line_)
{
	_state.is_end = true;
	return true;
}

bool kernel::dialang::_Cdelay(std::string& line_)
{
	//delete space and knowing is input - number
	while(isspace(line_[0])) line_ = line_.substr(1);
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

bool kernel::dialang::_Cwait(std::string& line_)
{
	//delete space and knowing is input - number
	while(isspace(line_[0])) line_ = line_.substr(1);
	if(!isdigit(line_[0])) return false;
	
	//parse, when digits is ended
	_state.wait = 0;
	while(isdigit(line_[0]))
	{
		_state.wait = _state.wait * 10 + ctodigit(line_[0]);
		line_ = line_.substr(1);
	}
	return true;
}

bool kernel::dialang::_Cgoto(std::string& line_)
{
	//delete space and knowing is input - number
	while(isspace(line_[0])) line_ = line_.substr(1);
	if(!isdigit(line_[0])) return false;

	//parse, when digits is ended and assign
	int buf = 0;
	while(isdigit(line_[0]))
	{
		buf = buf * 10 + ctodigit(line_[0]);
		line_ = line_.substr(1);
	}
	_state.line = buf;
	return true;
}

bool kernel::dialang::_Cfile(std::string& line_)
{
	//assign dir
	_state.dir = _divcommand(line_);

	//delete space and knowing is input - number
	while(isspace(line_[0])) line_ = line_.substr(1);
	if(!isdigit(line_[0])) return false;

	//parse, when digits is ended and assign
	int buf = 0;
	while(isdigit(line_[0]))
	{
		buf = buf * 10 + ctodigit(line_[0]);
		line_ = line_.substr(1);
	}
	_state.line = buf;
	return true;
}

bool kernel::dialang::_Ctask(std::string& line_)
{
	std::string addetive = _divcommand(line_);
	std::string name_task = _divcommand(line_);;
	if(addetive == "get")
	{
		auto element = _state.mission_stage.find(name_task);
		(*element).second.is_hotkey = true;
	}
	else if(addetive == "add")
	{
		auto element = _state.mission_stage.find(name_task);
		assert(((*element).second.is_hotkey));
		(*element).second.is_started = true;
	}
	else return false;
	return true;
}

bool kernel::dialang::_Cif(std::string& line_)
{
	// getting all properties about task and condition
	std::string name_task = _divcommand(line_);
	assert(((*_state.mission_stage.find(name_task)).second.is_hotkey));
	bool is_true = (*_state.mission_stage.find(name_task)).second.is_started ? (*_state.mission_stage.find(name_task)).second.is_completed : false;

	// detect operator
	while(isspace(line_[0])) line_ = line_.substr(1);
	if(line_[0] != _state.char_operator)
	{
		if(is_true) _state.line++;
		else _Cend(line_);
		return true;
	}
	for(auto iter : dalg::operatorlist)
	{
		auto croped = line_.substr(0,)
		(iter.name)
	}

	// complete command in if
	
	// detect else
	while(isspace(line_[0])) line_ = line_.substr(1);
	
	// complete command in else

	return false;
}

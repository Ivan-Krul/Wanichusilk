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
	_state.dir = divide(line_,_state.char_startcommand,_state.char_endcommand);

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
	std::string addetive = divide(line_, _state.char_startcommand, _state.char_endcommand);
	std::string name_task = divide(line_, _state.char_startcommand, _state.char_endcommand);
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
	std::string name_task = divide(line_, _state.char_startcommand, _state.char_endcommand);
	assert(((*_state.mission_stage.find(name_task)).second.is_hotkey));
	bool is_true = (*_state.mission_stage.find(name_task)).second.is_started ? (*_state.mission_stage.find(name_task)).second.is_completed : false;

	while(isspace(line_[0])) line_ = line_.substr(1);
	if(!isdigit(line_[0]))
	{
		if(is_true) _state.line++;
		else _Cend(line_);

		return true;
	}
	int gto = 0;
	while(isdigit(line_[0]))
	{
		gto = gto * 10 + ctodigit(line_[0]);
		line_ = line_.substr(1);
	}
	if(is_true) _state.line = gto;
	return true;
}

bool kernel::dialang::_Ctext(std::string& line_)
{
	_state.text_.author = divide(line_, _state.char_startcommand, _state.char_endcommand);
	_state.text_.emotion = divide(line_, _state.char_startcommand, _state.char_endcommand);
	std::string text = divide(line_, _state.char_string, _state.char_string);
	if(text.substr(text.size() - 2, 2) == "\n") 
		_state.text_.text += text;
	else 
		_state.text_.text = text;
	return true;
}

bool kernel::dialang::_Cchoose(std::string& line_)
{
	_state.choose_.author = divide(line_, _state.char_startcommand, _state.char_endcommand);
	while(!line_.empty())
		_state.choose_.links.push_back({ divide(line_,_state.char_string,_state.char_string),std::stoi(divide(line_,' ',' ')) });
	_state.choose_.is_chosen = false;
	return true;
}

bool kernel::dialang::_Cact(std::string& line_)
{
	_state.act_.name_act = divide(line_, _state.char_startcommand, _state.char_endcommand);
	return true;
}

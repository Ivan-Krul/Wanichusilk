#include "flag.h"

void mnlg::flag::_clear()
{
	if(_fbool != nullptr) (delete _fbool,_fbool = nullptr);
	if(_fchar != nullptr)(delete _fchar, _fchar = nullptr);
	if(_fint != nullptr) (delete _fint, _fint = nullptr);
	if(_ffloat != nullptr) (delete _ffloat, _ffloat = nullptr);
	if(_fstring != nullptr) (delete _fstring, _fstring = nullptr);
}

bool mnlg::flag::_isAllEmpty()
{
	return (_fbool == nullptr) &&
		(_fchar == nullptr) &&
		(_fint == nullptr) &&
		(_ffloat == nullptr) &&
		(_fstring == nullptr);
}

bool mnlg::flag::_isNumber(char chr_)
{
	return '0' <= chr_ <= '9';
}

int mnlg::flag::_number(char chr_)
{
	return chr_ - '0';
}

void mnlg::flag::_convertSToNum(std::string var_)
{
	// we are dealing with numbers
	bool isNegative = false;
	if(var_[0] == '-') isNegative = true;
	int i;
	bool isF = false;
	float f;
	int flagfloat = 0;
	for(int iter = isNegative; iter < var_.size(); iter++)
	{
		if(var_[iter] == '.' || var_[iter] == ',')
		{
			isF = true;
			flagfloat = iter;
		}
		else if(isF) f += (float)_number(var_[i]) * powf(10, flagfloat - iter); // 0.12 + 3 * 10^-3 = 0.123
		else
		{
			i *= 10; // 55555 * 10 + 7 = 555557
			i += _number(var_[i]);
		}
	}
	if(isF) _ffloat = new float((float)i + f * (!isNegative ? 1 : -1));
	else _fint = new int(i * (!isNegative ? 1 : -1));
}

mnlg::flag::flag()
{
	_fbool = nullptr;
	_fchar = nullptr;
	_fint = nullptr;
	_ffloat = nullptr;
	_fstring = nullptr;
}

void mnlg::flag::from_string(std::string var_)
{
	if(!_isAllEmpty()) _clear();
	bool isNegative = false;
	if(var_[0] == '-') isNegative = true;
	if(_isNumber(var_[isNegative])) _convertSToNum(var_);
	else if(var_[0] == '\'') _fchar = new char (var_[1]); // we are dealing with char
	else if(var_[0] == '"') _fstring = new std::string(var_.substr(1, var_.size() - 2)); // we are dealing with string
	else _fbool = new bool(var_[0] == 't'); // we are dealing with bool
}

bool mnlg::flag::fbool()
{
	return *_fbool;
}

char mnlg::flag::fchar()
{
	return *_fchar;
}

int mnlg::flag::fint()
{
	return *_fint;
}

float mnlg::flag::ffloat()
{
	return *_ffloat;
}

std::string mnlg::flag::fstring()
{
	return *_fstring;
}

void mnlg::flag::fbool(bool a)
{
	if(_fbool == nullptr) _fbool = new bool(a);
	else *_fbool = a;
}

void mnlg::flag::fchar(char a)
{
	if(_fchar == nullptr) _fchar = new char(a);
	else *_fchar = a;
}

void mnlg::flag::fint(int a)
{
	if(_fint == nullptr) _fint = new int(a);
	else *_fint = a;
}

void mnlg::flag::ffloat(float a)
{
	if(_ffloat == nullptr) _ffloat = new float(a);
	else *_ffloat = a;
}

void mnlg::flag::fstring(std::string a)
{
	if(_fstring == nullptr) _fstring = new std::string(a);
	else *_fstring = a;
}

mnlg::flag::~flag()
{
	_clear();
}

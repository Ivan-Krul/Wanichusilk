#pragma once
#include <string>

namespace mnlg
{

	class flag
	{
		bool* _fbool;
		char* _fchar;
		int* _fint;
		float* _ffloat;
		std::string* _fstring;
		
		void _clear();
		bool _isAllEmpty();
		bool _isNumber(char chr_);
		int _number(char chr_);
		void _convertSToNum(std::string var_);
	public:
		flag();
		void from_string(std::string var_);

		bool fbool();
		char fchar();
		int fint();
		float ffloat();
		std::string fstring();

		~flag();
	};
}

#include "flag.cpp"
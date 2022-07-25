#pragma once
#include "../charity/charity.h"
#include "../hardware/hardware.h"

class monolog {
protected:
	name* _author = nullptr;
	std::string _words = "[word]";
public:
	monolog() {}
	monolog(name& author_) { create(author_); }
	monolog(name& author_, std::string word_) { create(author_, word_); }
	void create(name& author_) { _author = &author_; }
	void create(name& author_, std::string word_);

	void write(std::string words_) { _words = words_; }
	name author() { return *_author; }

	std::string operator() (); //
};

#include "monolog.cpp"
#pragma once
#include "../charity/charity.h"

__interface Imonolog {
	void create(name& author_);
	void create(name& author_, std::string word_);
	void create(std::string word_);

	void write(std::string words_);
	name author();

	std::string get_monolog();
};

class monolog {
protected:
	name* _author = nullptr;
	std::string _words = "[word]";
public:
	monolog() {}
	monolog(name& author_) { create(author_); }
	monolog(name& author_, std::string word_) { create(author_, word_); }
	monolog(std::string word_) { create(word_); }
	void create(name& author_) { _author = &author_; }
	void create(name& author_, std::string word_);
	void create(std::string word_) { _words = word_; }

	void write(std::string words_) { _words = words_; }
	name author() { return *_author; }

	std::string get_monolog(); //
};

#include "monolog.cpp"
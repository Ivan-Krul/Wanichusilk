#pragma once
#include "../charity/charity.h"
#include "../../abstractes/loader.h"

__interface Imonolog {
	void create(name& author_);
	void create(name& author_, std::string word_);
	void create(std::string word_);

	void write(std::string words_);
	name author();

	std::string get_monolog();
};

class kernel_monolog : public loader {
protected:
	std::string _author = "[author]";
	std::string _words = "[word]";
public:
	kernel_monolog() {}
	kernel_monolog(std::string author_, std::string word_) { create(author_, word_); }
	kernel_monolog(std::string word_) { create(word_); }
	void create(std::string author_, std::string word_);
	void create(std::string word_) { _words = word_; }

	void write(std::string words_) { _words = words_; }
	std::string author() { return _author; }
	void load(std::string dir_, size_t line_);
	void parse(std::string raw_);

	std::string get_monolog(); //
};

#include "monolog.cpp"
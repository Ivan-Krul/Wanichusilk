#pragma once
#include "../charity/charity.h"

class monolog {
	const name* _author;
	std::string _words = "[word]";
public:
	monolog() {}
	monolog(const name& author_) { create(author_); }
	monolog(const name& author_, std::string word_) { create(author_, word_); }
	void create(const name& author_) { _author = author_; }
	void create(const name& author_, std::string word_);

	void write(std::string words_) { _words = words_; }
	void read_from_disk(std::string dir_, size_t start_,size_t size_);
	const name author() { return *_author; }

	std::string operator() () { return _words; }
};

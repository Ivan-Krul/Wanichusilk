#include "monolog.h"



void monolog::create(name& author_, std::string word_) {
	_author = &author_;
	_words = word_;
}

std::string monolog::operator() () {
	std::string str;
	if (_author != nullptr) str = _author->get_name();
	else str = "?";
	str += ": " + _words;

	return str;
}

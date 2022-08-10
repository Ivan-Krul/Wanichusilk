#include "monolog.h"

void kernel_monolog::create(name& author_, std::string word_) {
	_author = &author_;
	_words = word_;
}

std::string kernel_monolog::get_monolog() {
	std::string str;
	if (_author != nullptr) str = _author->get_name();
	else str = "?";
	str += ": " + _words;

	return str;
}

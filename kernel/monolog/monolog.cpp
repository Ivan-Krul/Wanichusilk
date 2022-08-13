#include "monolog.h"

void kernel_monolog::create(std::string author_, std::string word_) {
	_author = author_;
	_words = word_;
}

void kernel_monolog::load(std::string dir_, size_t line_)
{
	kernel_hardware hardware;
	hardware.read_to_buffer_ln(dir_, line_);
	std::string raw = hardware.buffer();

	parse(raw);
}

void kernel_monolog::parse(std::string raw_)
{
	checkParse(raw_, _author);
	checkParse(raw_, _words);
}

std::string kernel_monolog::get_monolog() {
	std::string str;
	if(_author == "[action]") str = "";
	else if(_author == "[?]") str = "[?]: ";
	else str = str=_author+": ";
	str += _words;

	return str;
}

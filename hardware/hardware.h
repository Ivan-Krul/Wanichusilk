#pragma once
#include "../define.h"
#include <fstream>

__interface Ihardware {
	void read_to_buffer(const std::string dir_, const size_t beg_, const size_t size_);
	void write_from_buffer(const std::string dir_, const size_t beg_);
	std::string& buffer();
};

class basic_hardware : public Ihardware {
	std::string _buf;
public:
	void read_to_buffer(const std::string dir_, const size_t beg_, const size_t size_);
	void write_from_buffer(const std::string dir_, const size_t beg_);

	std::string& buffer();
	
	template <typename T> T convert_from_buffer();
};


#include "hardware.cpp"

template<typename T>
inline T basic_hardware::convert_from_buffer() {
	size_t size = sizeof(T);

	T a;
	char* pt = &a;

	for (size_t i = 0;i < size;i++) {
		*pt = _buf[i];
		pt++;
	}

	_buf.substr(size);

	return a;
}

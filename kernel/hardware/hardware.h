#pragma once
#include "../../define.h"
#include <fstream>
#include <string>

class basic_hardware
{
protected:
	std::string _buf;

	std::ofstream _openFileO(const std::string dir_);
	std::ifstream _openFileI(const std::string dir_);

	uint32_t _getSizeFile(const std::string dir_);
public:
	void read_to_buffer(const std::string dir_);
	void write_from_buffer(const std::string dir_);

	void read_to_buffer_ln(const std::string dir_, size_t line_);
	void write_from_buffer_ln(const std::string dir_, const size_t beg_);

	bool is_open(const std::string dir_);

	std::string& buffer();
	
	template <typename T> T read_binary(const std::string dir_, const size_t beg_);
	template <typename T> void write_binary(const std::string dir_, const size_t beg_, T thing_);
	template <typename T> T convert_from_buffer();
};

std::string& basic_hardware::buffer()
{
	return _buf;
}

#include "hardware.cpp"
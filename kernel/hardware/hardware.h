#pragma once
#include "../../define.h"
#include <fstream>
#include <string>

__interface Ihardwre
{
	void read_to_buffer(const std::string dir_);
	void read_to_buffer_line(const std::string dir_, const size_t line_);

	void write_from_buffer(const std::string dir_);
	void write_from_buffer_line(const std::string dir_, const size_t beg_);

	std::string& buffer();
};

class basic_hardware : public Ihardwre
{
protected:
	std::string _buf;

	std::ofstream _openFileO(const std::string dir_);
	std::ifstream _openFileI(const std::string dir_);
public:
	void read_to_buffer(const std::string dir_);
	void write_from_buffer(const std::string dir_);

	void read_to_buffer_line(const std::string dir_, const size_t line_);
	void write_from_buffer_line(const std::string dir_, const size_t beg_);

	std::string& buffer();
	
	template <typename T> T convert_from_buffer();
};

std::string& basic_hardware::buffer()
{
	return _buf;
}

#include "hardware.cpp"
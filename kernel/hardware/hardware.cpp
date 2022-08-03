#include "hardware.h"

std::ofstream basic_hardware::_openFileO(const std::string dir_)
{
	std::ofstream ofs;
	ofs.open(dir_);

	assert(!ofs.fail());
}

std::ifstream basic_hardware::_openFileI(const std::string dir_)
{
	std::ifstream ifs;
	ifs.open(dir_);

	assert(ifs.is_open());
	assert(!ifs.fail());
}

void basic_hardware::read_to_buffer(const std::string dir_)
{
	auto ifs = _openFileI(dir_);
	ifs.open(dir_);
	_buf.resize(ifs.tellg());
	ifs.get((char*)_buf.c_str(), _buf.size());
	ifs.close();
}

void basic_hardware::write_from_buffer(const std::string dir_)
{
	auto ofs = _openFileO(dir_);
	ofs.open(dir_);
	ofs.write(_buf.c_str(), _buf.size());
	ofs.close();
}

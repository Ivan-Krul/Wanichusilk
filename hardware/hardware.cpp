#include "hardware.h"

void basic_hardware::read_to_buffer(const std::string dir_, const size_t beg_, const size_t size_) {
	std::ifstream ifs;
	_buf.resize(size_);
	ifs.open(dir_);

	ifs.seekg(beg_, std::ios_base::beg);
	ifs.get((char*)_buf.c_str(), size_);

	ifs.close();
}

void basic_hardware::write_from_buffer(const std::string dir_, const size_t beg_) {
	std::ofstream ofs;
	ofs.open(dir_);

	ofs.seekp(beg_, std::ios_base::beg);
	ofs.write(_buf.c_str(), _buf.size());

	ofs.close();
}

std::string& basic_hardware::buffer() {
	return _buf;
}

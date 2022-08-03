#include "hardware.h"

void basic_hardware::read_to_buffer_line(const std::string dir_, size_t line_)
{
	auto ifs = _openFileI(dir_);
	for(size_t i = 0; i < line_ && ifs.eof(); i++)
	{
		std::getline(ifs, _buf);
	}
	ifs.close();
}
void basic_hardware::write_from_buffer_line(const std::string dir_, const size_t beg_)
{
	auto ofs = _openFileO(dir_);
	ofs.seekp(beg_, std::ios_base::beg);
	ofs << _buf<<'\n';
	ofs.close();
}

template<typename T>
T basic_hardware::convert_from_buffer()
{
	size_t size = sizeof(T);
	T a;
	char* pt = &a;
	for(size_t i = 0; i < size; i++)
	{
		*pt = _buf[i];
		pt++;
	}
	_buf.substr(size);
	return a;
}


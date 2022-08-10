#include "hardware.h"

std::ofstream kernel_hardware::_openFileO(const std::string dir_)
{
	std::ofstream ofs;
	ofs.open(dir_);

	assert(!ofs.fail());
	return ofs;
}

std::ifstream kernel_hardware::_openFileI(const std::string dir_)
{
	std::ifstream ifs;
	ifs.open(dir_);

	assert(ifs.is_open());
	assert(!ifs.fail());
	return ifs;
}

uint32_t kernel_hardware::_getSizeFile(const std::string dir_)
{
	std::ifstream in(dir_, std::ifstream::ate | std::ifstream::binary);
	uint32_t size = in.tellg();
	in.close();
	return size;
}

void kernel_hardware::read_to_buffer(const std::string dir_)
{
	uint32_t size = _getSizeFile(dir_);

	auto ifs = _openFileI(dir_);
	_buf.resize(size);
	ifs.get((char*)_buf.c_str(), size,'\0');
	ifs.close();
}

void kernel_hardware::write_from_buffer(const std::string dir_)
{
	auto ofs = _openFileO(dir_);
	ofs.write(_buf.c_str(), _buf.size());
	ofs.close();
}

void kernel_hardware::read_to_buffer_ln(const std::string dir_, size_t line_)
{
	auto ifs = _openFileI(dir_);
	for(size_t i = 0; i < line_ && !ifs.eof(); i++)
	{
		std::getline(ifs, _buf);
	}
	ifs.close();
}
void kernel_hardware::write_from_buffer_ln(const std::string dir_, const size_t beg_)
{
	auto ofs = _openFileO(dir_);
	ofs.seekp(beg_, std::ios_base::beg);
	ofs << _buf << '\n';
	ofs.close();
}

bool kernel_hardware::is_open(const std::string dir_)
{
	std::ifstream ofs;
	ofs.open(dir_);
	if(ofs.is_open())
	{
		ofs.close();
		return true;
	}
	else return false;
}

template<typename T>
T kernel_hardware::read_binary(const std::string dir_, const size_t beg_)
{
	T what;
	auto ifs = std::ifstream();
	ifs.open(dir_, std::ios_base::binary);

	ifs.seekg(beg_, std::ios_base::beg);
	ifs >> what;

	ifs.close();
	return what;
}

template<typename T>
void kernel_hardware::write_binary(const std::string dir_, const size_t beg_, T thing_)
{
	auto ofs = std::ofstream();
	ofs.open(dir_, std::ios_base::binary);

	ofs.seekp(beg_, std::ios_base::beg);
	ofs << thing_;

	ofs.close();
}

template<typename T>
T kernel_hardware::convert_from_buffer()
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

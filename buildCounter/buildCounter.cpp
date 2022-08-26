#include "buildCounter.h"

buildCounter::buildCounter()
{
	kernel::hardware hardware;
	if(hardware.is_open("buildCount.txt"))
	{
		hardware.read_to_buffer_ln("buildCount.txt",1);
		_buildNumber = std::stoul(hardware.buffer());
		_buildNumber++;
	}
	else
	{
		hardware.write_from_buffer("buildCount.txt");
	}
	hardware.buffer() = std::to_string(_buildNumber);
	hardware.write_from_buffer("buildCount.txt");
}

size_t buildCounter::get()
{
	return _buildNumber;
}

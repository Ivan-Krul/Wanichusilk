#include "buildCounter.h"

buildCounter::buildCounter()
{
	basic_hardware hardware;
	hardware.read_to_buffer_ln("buildCount.txt",1);
	_buildNumber = std::stoul(hardware.buffer());
	_buildNumber++;
	hardware.buffer() = std::to_string(_buildNumber);
	hardware.write_from_buffer("buildCount.txt");
}

size_t buildCounter::get()
{
	return _buildNumber;
}

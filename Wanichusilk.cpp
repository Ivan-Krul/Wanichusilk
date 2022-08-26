// Wanichusilk.cpp : Defines the entry point for the application.
//

#include "Wanichusilk.h"
#include <thread>
#include <chrono>

int main()
{
	buildCounter build;
	std::cout << "[Wanichusilk]" << std::endl;
	std::cout <<"build: " << build.get() << std::endl;

	kernel::dialang dial;
	dial.test();
	//dial.start("D:\\Progect\\Visual Studio 2019\\C++\\Wanichusilk\\resources\\dialang.das");
	//while(!dial.state().is_end) dial.step();
	return 0;
}

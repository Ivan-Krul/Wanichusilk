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
	dial.start("../../../resources/dialang.das");
	//dial.test();
	dial.load_task("task #1", false, false);
	auto state = dial.state();
	while(!state.is_end)
	{
		state = dial.state();
		if(state.out == kernel::dalg::type_out::text)
		{
			std::cout << state.text_.author << " *" << state.text_.emotion << "*:" << state.text_.txt << '\n';
		}
		else if(state.out == kernel::dalg::type_out::choose)
		{
			std::cout << state.choose_.author << ":\n";
			for(int i = 0;i < state.choose_.links.size(); i++)
			{
				std::cout << "\t" << i << ": " << state.choose_.links[i].first << '\n';
			}
			int what;
			std::cout << "What you would to choose: ";
			std::cin >> what;
			what %= state.choose_.links.size();
			state.choose_.choice = what;
			state.choose_.is_chosen = true;
			dial.load_choose(state.choose_);
		}
		//else std::cout << state.line << '\n';
		dial.step();
	}

	//dial.start("D:\\Progect\\Visual Studio 2019\\C++\\Wanichusilk\\resources\\dialang.das");
	//while(!dial.state().is_end) dial.step();
	return 0;
}

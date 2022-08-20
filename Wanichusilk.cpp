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

	std::cout << typeid(std::string).name() << '\n';

	kernel_charity charit;
	kernel_item some;
	some.load("../../../resources/item.rctxt", 1);

	charit.load("../../../resources/charity.rctxt", 1);

	std::cout << charit.get_description() << std::endl;
	std::cout <<some.get_description() << std::endl;

	monolang mnlg;
	mnlg.start("../../../resources/monolang.mns");
	
	while(!mnlg.is_end() || !mnlg.is_aborted())
	{
		mnlg.step();
		if(mnlg.is_log()) std::cout << mnlg.log();
		else if(mnlg.is_choose())
		{
			monochoice choice = mnlg.choose();
			for(size_t i = 0; i < choice.size();i++) std::cout <<i<< ": " << choice.variant(i) << std::endl;
			int var;
			std::cin >> var;
			choice.choose(var);
			mnlg.choice(choice);
		}
		else if(mnlg.is_flag())
		{
		}
		if(mnlg.is_aborted()) throw std::bad_typeid();
		std::cout << "\tlog: " << mnlg.logger() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(mnlg.delay()));
	}
	if(mnlg.is_aborted()) std::cout << mnlg.errorHandler() << std::endl;
	return 0;
}

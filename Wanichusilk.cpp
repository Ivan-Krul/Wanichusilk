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
	
	while(!mnlg.access()._isEnd || !mnlg.access()._isAborted)
	{
		mnlg.step();
		if(mnlg.access()._isLog) std::cout << mnlg.log();
		else if(mnlg.access()._isChoose)
		{
			monochoice choice = mnlg.choose();
			for(size_t i = 0; i < choice.size();i++) std::cout <<i<< ": " << choice.variant(i) << std::endl;
			int var;
			std::cin >> var;
			choice.choose(var);
			mnlg.choice(choice);
		}
		else if(mnlg.access()._isFlag)
		{
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(mnlg.delay()));
	}
	if(mnlg.access()._isAborted) std::cout << mnlg.errorHandler() << std::endl;
	return 0;
}

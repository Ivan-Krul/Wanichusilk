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
	try
	{
		kernel_charity charit;
		kernel_item some;
		some.load("../../../resources/item.rctxt", 1);

		charit.load("../../../resources/charity.rctxt", 1);

		std::cout << charit.get_description() << std::endl;
		std::cout <<some.get_description() << std::endl;

		monolang mnlg;
		mnlg.start("../../../resources/monolang.mns");
	
		while(!mnlg.end())
		{
			mnlg.step();
			if(mnlg.is_log()) std::cout<<mnlg.get_log()<<std::endl;
			else if(mnlg.is_choose())
			{
				monochoice choice = mnlg.get_choose();
				for(size_t i = 0; i < choice.size();i++) std::cout <<i<< ": " << choice.variant(i) << std::endl;
				int var;
				std::cin >> var;
				choice.choose(var);
				mnlg.get_choice(choice);
			}
			else if(mnlg.is_flag())
			{
				switch(mnlg.typedata_flag())
				{
					case stateTypeFlag::INT:
						std::cout << mnlg.get_flag((int)1) << std::endl;
						break;
					case stateTypeFlag::STRING:
						std::cout << mnlg.get_flag(std::string()) << std::endl;
					default:
						break;
				}
			}
			if(mnlg.aborted()) throw std::bad_typeid();
			std::this_thread::sleep_for(std::chrono::milliseconds(mnlg.delay()));
		}
		if(mnlg.aborted()) std::cout << mnlg.errorHandler() << std::endl;
		return 0;

	}
	catch(const std::exception&)
	{
		std::cout << "exception!\n";
		system("mp4.mp4");
	}
}

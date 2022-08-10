// Wanichusilk.cpp : Defines the entry point for the application.
//

#include "Wanichusilk.h"

int main()
{
	buildCounter build;
	std::cout << "[Wanichusilk]" << std::endl;
	std::cout <<"build: " << build.get() << std::endl;
	//kernel_charity charit[2];
	//charit[0].create("Jeff", 4, 2);
	//charit[1].create("Brebius", 4, 3);

	//monolog mon[6];
	//mon[0].create(charit[0], "what are you doing?");
	//mon[1].create(charit[1], "tee hee...");
	//mon[2].create(charit[0], "go away and don't disturbe me!");
	//mon[3].create(charit[0], "should me fight with you?");
	//mon[4].create(charit[1], "... yes");
	//mon[5].create("wow");
	//for (size_t i = 0;i < std::size(mon); i++) {
	//	std::cout << mon[i].get_monolog() << '\n';
	//}
	//std::cout<<"you damaged " << charit[0].damage(charit[1])<<" to "<<charit[1].get_name()<<'\n';
	//std::cout << monolog(charit[1], "ow").get_monolog()<<'\n';

	//sandwich san; 
	//std::cout << "there laying a " << san.get_name() << "\nit " << san.get_description()<<'\n';

	//basic_hardware hardware;
	//hardware.buffer() = "sandwich|5|e|simple combination of a bread, salad, ham and souces, borrowed from Heavy|\n";
	//hardware.buffer() += "banana|3|e|fruit for fitness, filled potassium|\n";
	//hardware.buffer() += "helmet|4|a|very huge and very massive|\n";
	//hardware.buffer() += "bonzai vest|15|a|armorness vest, made with bonzai leaves|\n";
	//hardware.buffer() += "stick|2|w|fresh, from branch|\n";
	//hardware.buffer() += "toy knife|11|w|from kitchen toy kit|\n\0";
	//hardware.write_from_buffer("../../../resources/item.rctxt");

	//hardware.buffer() = "";

	//hardware.read_to_buffer("../../../resources/item.rctxt");
	//std::cout<<hardware.buffer();

	item some;
	some.load("../../../resources/item.rctxt", 2);

	std::cout <<some.description() << std::endl;

	return 0;
}

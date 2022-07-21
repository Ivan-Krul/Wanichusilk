// Wanichusilk.cpp : Defines the entry point for the application.
//

#include "Wanichusilk.h"

int main()
{
	charity charit("Jeff",4,2);
	charity charitt("Brebius", 4, 3);
	std::cout<<charit.get_name()<<'\n';
	std::cout << charit.info() << '\n';
	std::cout << charitt.info() << '\n';
	std::cout << charit.prod() << ' ' << charitt.prod()<< '\n';
	std::cout << charit.damage(charitt)<<'\n';
	std::cout << charit.prod() << ' ' << charitt.prod()<<'\n';

	return 0;
}

#ifdef UNIT_TEST
#include "Utility.h"
#include <iostream>

using namespace utl;

int main(int argc, char* argv[])
{
	std::cout << GetInput<int>("input number:") << std::endl;
	std::cout << GetInput<std::string>("input string:") << std::endl;

    return 0;
}

#endif

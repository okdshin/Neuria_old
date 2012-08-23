#ifdef HASH_UNIT_TEST
#include "Hash.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	std::cout << GetHashStr(utl::CreateByteArray("string")) << std::endl;
	std::cout << GetHashStr(utl::CreateByteArray("strinh")) << std::endl;
	std::cout << GetHashStr(utl::CreateByteArray("")) << std::endl;
    return 0;
}

#endif

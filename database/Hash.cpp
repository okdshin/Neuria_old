#ifdef HASH_UNIT_TEST
#include "Hash.h"
#include <iostream>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	std::cout << CalcHashStr(utl::String2ByteArray("string")) << std::endl;
	std::cout << CalcHashStr(utl::String2ByteArray("strinh")) << std::endl;
	std::cout << CalcHashStr(utl::String2ByteArray("")) << std::endl;
    return 0;
}

#endif

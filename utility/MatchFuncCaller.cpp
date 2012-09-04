#ifdef MATCHFUNCCALLER_UNIT_TEST
#include "MatchFuncCaller.h"
#include "Utility.h"
#include <iostream>

using namespace nr;
using namespace nr::utl;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	
	auto controller = MatchFuncCaller<int>::Create(service, 
		[](const ByteArray&){return 10;});

	controller->Register(10, 
		[](const ByteArray&){std::cout << "hello" << std::endl;});

	controller->Register(20, 
		[](const ByteArray&){std::cout << "noon" << std::endl;});

	controller->CallMatchFunc(utl::String2ByteArray("hello"));
	
	service.run();
    return 0;
}

#endif

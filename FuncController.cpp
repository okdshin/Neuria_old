#ifdef FUNCCONTROLLER_UNIT_TEST
#include "FuncController.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	
	auto controller = FuncController<int>::Create(service, 
		[](const utl::ByteArray&){return 10;});

	controller->Register(10, 
		[](Session::Pointer, const utl::ByteArray&){std::cout << "hello" << std::endl;});

	controller->Register(20, 
		[](Session::Pointer, const utl::ByteArray&){std::cout << "noon" << std::endl;});

	auto session = CreateTestSession(service);
	controller->CallMatchFunc(session, utl::String2ByteArray("hello"));
	
	service.run();
    return 0;
}

#endif

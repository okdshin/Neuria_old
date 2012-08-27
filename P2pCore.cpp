#ifdef P2PCORE_UNIT_TEST
#define DEBUG_LEVEL 60
#include "P2pCore.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include "Utility.h"

using namespace nr;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	int server_port = 54321;
	if(argc == 2)
	{
		server_port = boost::lexical_cast<int>(argv[1]);
	}

	const int buffer_size = 128;
	auto core_ptr = P2pCore::Create(service, server_port, buffer_size, 
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from upper
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM UPPER called:" << str << std::endl;
		},
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from lower
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM LOWER called:" << str << std::endl;
			//core->BroadcastToUpper(byte_array);
		},
		(std::cout)
	);

	std::cout << "accept port is " << server_port << std::endl;
	
	P2pCoreTestCuiApp(service, core_ptr);

    return 0;
}
#endif

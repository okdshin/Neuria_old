#ifdef NODE_UNIT_TEST
#include "Node.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	int server_port = 54321;
	if(argc == 2)
	{
		server_port = boost::lexical_cast<int>(argv[1]);
	}

	auto node = Node<int>(service, server_port, 128, 
		[](const utl::ByteArray&)->int{return 10;}, std::cout);
	
	node.RegisterFuncForUpper(10,
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from upper
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM UPPER called:" << str << std::endl;
			std::cout << core->GetSessionListStr() << std::endl;
		}
	);
	node.RegisterFuncForLower(10,
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from lower
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM LOWER called:" << str << std::endl;
			core->BroadcastToUpper(byte_array);
		}
	);

	auto core_ptr = node.GetCorePtr();
	std::cout << "accept port is " << server_port << std::endl;
	
	P2pCoreTestCuiApp(service, core_ptr);

    return 0;
}

#endif

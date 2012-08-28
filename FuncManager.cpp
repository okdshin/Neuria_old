#ifdef FUNC_MANAGER_UNIT_TEST
#include "FuncManager.h"
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

	const int buffer_size = 128;

	FuncManager<std::string>::FuncDict for_upper_func_dict{}; 
	for_upper_func_dict["command1"] = 
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from upper
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAND1111:onreceive FROM UPPER called:" << str << std::endl;
			std::cout << core->GetSessionListStr() << std::endl;
	};
	for_upper_func_dict["command2"] = 
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from upper
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAND2222:onreceive FROM UPPER called:" << str << std::endl;
			std::cout << core->GetSessionListStr() << std::endl;
	};
	
	FuncManager<std::string>::FuncDict for_lower_func_dict{};
	for_lower_func_dict["command1"] = 
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from lower
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAND1111:onreceive FROM LOWER called:" << str << std::endl;
			std::cout << core->GetSessionListStr() << std::endl;
	};
	
	for_lower_func_dict["command2"] = 
		[](P2pCore::Pointer core, Session::Pointer session, 
				const utl::ByteArray& byte_array){ //on receive func from lower
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAN2222:Donreceive FROM LOWER called:" << str << std::endl;
			std::cout << core->GetSessionListStr() << std::endl;
	};
	
	auto func_manager_ptr = FuncManager<std::string>::Create(service, server_port, buffer_size, 
		[](const utl::ByteArray& byte_array)->std::string{
			return byte_array.size()%2 ? "command1" : "command2";
		}, 
		for_upper_func_dict, for_lower_func_dict, std::cout);

	auto core_ptr = func_manager_ptr->GetCorePtr();
	std::cout << "accept port is " << server_port << std::endl;
	
	P2pCoreTestCuiApp(service, core_ptr);

    return 0;
}

#endif

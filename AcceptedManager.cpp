#ifdef ACCEPTEDMANAGER_UNIT_TEST
#include "AcceptedManager.h"
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

	auto accepted_pool_ptr = SessionPool::Create();
	auto accepted_manager_ptr = AcceptedManager<std::string>::Create(
		service, server_port, buffer_size, accepted_pool_ptr,
		[](const utl::ByteArray& byte_array)->std::string{
			return byte_array.size()%2 ? "command1111" : "command2222";
		}, std::cout);
	
	accepted_manager_ptr->Register("command1111",
		[](Session::Pointer session, const utl::ByteArray& byte_array){
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAND1111:onreceive FROM UPPER called:" << str << std::endl;
	});
	
	accepted_manager_ptr->Register("command2222",
		[](Session::Pointer session, 
				const utl::ByteArray& byte_array){
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "COMMAND2222:onreceive FROM UPPER called:" << str << std::endl;
	});
	
	auto core_ptr = accepted_manager_ptr->GetCorePtr();
	std::cout << "accept port is " << server_port << std::endl;
	
	P2pCoreTestCuiApp(service, core_ptr);

    return 0;
}
#endif

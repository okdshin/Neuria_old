#ifdef P2PCORE_UNIT_TEST
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
	auto session_pool = SessionPool::Create();
	auto core_ptr = P2pCore::Create(service, server_port, buffer_size, 
		[&session_pool](Session::Pointer session){
			std::cout << "on_accept_func called:" << std::endl;
			session_pool->Add(session);
		},
		[](Session::Pointer session, const utl::ByteArray& byte_array){ 
			std::cout << "on receive from accepted session:" 
				<< utl::ByteArray2String(byte_array) << std::endl;
		},
		[&session_pool](Session::Pointer session){
			session_pool->Erase(session);
		},
		(std::cout)
	);

	std::cout << "accept port is " << server_port << std::endl;
	P2pCoreTestCuiApp(service, core_ptr, 
		[&session_pool](Session::Pointer session){  // on_connect
			std::cout << "on_connect!!!" << std::endl; 
			session_pool->Add(session);
		}, 
		[](Session::Pointer session, const utl::ByteArray& byte_array){ // on_receive
			std::cout << "on_receive!!!" << std::endl; 
		},
		[&session_pool](Session::Pointer session){ // on_close
			session_pool->Erase(session);
		},
		[&service, &core_ptr, &session_pool](const utl::ByteArray& byte_array){ //broadcast
			//Broadcast(service, session_pool, byte_array);
			Send(core_ptr, "127.0.0.1", 54321, utl::String2ByteArray("hello"));
		},
		[&session_pool](){ // close 
			std::cout << "close" << std::endl; 
			for(auto& session : *session_pool){
				session->Close();	
			}
		}
	);

    return 0;
}
#endif

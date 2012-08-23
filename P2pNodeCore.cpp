#ifdef P2PNODECORE_UNIT_TEST
#include "P2pNodeCore.h"
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

	P2pNodeCore node(service, server_port, 
		[](Session::Pointer session, const utl::ByteArray& byte_array){ //on receive func
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive called:" << str << std::endl;
			//session->Send(byte_array);
		}
	);

	std::cout << "accept port is " << server_port << std::endl;
	
	boost::thread t(boost::bind(&boost::asio::io_service::run, &service));
	while(true){ //main loop
		try{
			const auto command = utl::GetInput<std::string>("command?:");
			if(command == "connect"){
				const auto hostname = utl::GetInput<std::string>("hostname?:");
				const auto port = utl::GetInput<int>("port?:");	

				node.Connect(hostname, port);
			}	
			else if(command == "broadcast"){
				const auto message = utl::GetInput<std::string>("message?:");
				std::vector<char> msg(message.c_str(), message.c_str()+message.length());
				node.Broadcast(msg);
			}
			else if(command == "close"){
				const auto session_index = utl::GetInput<unsigned int>("sesion index?:");
				node.CloseSession(session_index);
			}
			else if(command == "session"){
				std::cout << node.GetSessionListStr() << std::endl;
			}
			else if(command == "exit" || command == "quit"){
				//std::cout << node.GetSessionListStr() << std::endl;
				exit(0);
			}
			else{
				std::cout << "invalid command." << std::endl;	
			}
		}
		catch(std::exception& e){
			std::cout << "error!!!:" << e.what() << std::endl;	
		}
	}
	t.join();
	

    return 0;
}
#endif

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

	auto core_ptr = P2pCore::Create(service, server_port, 
		[](Session::Pointer session, const utl::ByteArray& byte_array){ //on receive func
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM UPPER called:" << str << std::endl;
			//session->Send(byte_array);
		},
		[](Session::Pointer session, const utl::ByteArray& byte_array){ //on receive func
			std::string str(byte_array.begin(), byte_array.end());
			std::cout << "onreceive FROM LOWER called:" << str << std::endl;
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

				core_ptr->Connect(hostname, port);
			}	
			else if(command == "broadcast"){
				const auto message = utl::GetInput<std::string>("message?:");
				std::vector<char> msg(message.c_str(), message.c_str()+message.length());
				core_ptr->BroadcastToUpper(msg);
				core_ptr->BroadcastToLower(msg);
			}
			else if(command == "upper")
			{
				const auto message = utl::GetInput<std::string>("message?:");
				std::vector<char> msg(message.c_str(), message.c_str()+message.length());
				core_ptr->BroadcastToUpper(msg);	
			}
			else if(command == "lower")
			{
				const auto message = utl::GetInput<std::string>("message?:");
				std::vector<char> msg(message.c_str(), message.c_str()+message.length());
				core_ptr->BroadcastToLower(msg);	
			}
			else if(command == "close"){
				const auto which = utl::GetInput<std::string>("upper or lower?:");
				if(which != "upper" || which != "lower"){
					std::cout << 
						"invalid.(please input \"upper\" or \"lower\")" << std::endl;	
				}
				else{
					const auto session_index = 
						utl::GetInput<unsigned int>("sesion index?:");
					core_ptr->CloseLowerSession(session_index);	
				}
			}
			else if(command == "session"){
				std::cout << core_ptr->GetSessionListStr() << std::endl;
			}
			else if(command == "exit" || command == "quit"){
				//std::cout << core_ptr->GetSessionListStr() << std::endl;
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

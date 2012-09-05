#ifdef CLIENT_UNIT_TEST
#include "Client.h"
#include <iostream>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::ntw;

auto ClientTestCuiApp(boost::asio::io_service& service, Client::Pointer client, 
		Client::OnConnectFunc on_connect_func, 
		Session::OnReceiveFunc on_receive_func,
		Session::OnCloseFunc on_close_func,
		boost::function<void (const ByteArray&)> broadcast_func,
		boost::function<void ()> close_func) -> void {

	boost::asio::io_service::work w(service);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &service));
	std::cout << "\"help\" shows commands" << std::endl;
	while(true){ //main loop
		try{
			const auto command = utl::GetInput<std::string>("command?:");

			if(command == "help")
			{
				std::cout
					<< "!!!HELP!!!\n"
					<<"you can use below commands.\n"
					<< "connect:connect to upper peer.\n"
					<< "broadcast:broadcast message.\n"
					<< "close:close session.\n"
					<< "exit:exit app.\n"
					<< std::endl;
				
			}else if(command == "connect"){
				const auto hostname = utl::GetInput<std::string>("hostname?:");
				const auto port = utl::GetInput<int>("port?:");	
				std::cout << hostname << ":" << port << std::endl;
				client->Connect(hostname, port, 
					on_connect_func, on_receive_func, on_close_func);
			}
			else if(command == "broadcast"){
				const auto message = utl::GetInput<std::string>("message?:");
				std::vector<char> msg(message.c_str(), message.c_str()+message.length());
				broadcast_func(msg);
			}
			else if(command == "close"){
				close_func();	
			}
			else if(command == "exit"){
				exit(0);
			}
			else{
				std::cout << "invalid command.(command \"help\" and see)" << std::endl;	
			}
		}
		catch(std::exception& e){
			std::cout << "error!!!:" << e.what() << std::endl;	
		}
	}
	t.join();	
}

int main(int argc, char* argv[])
{
	boost::asio::io_service service;

	const int buffer_size = 128;
	auto session_pool = SessionPool::Create();
	auto client = Client::Create(service, buffer_size, std::cout);

	ClientTestCuiApp(service, client, 
		[&session_pool](Session::Pointer session){  // on_connect
			std::cout << "on_connect!!!" << std::endl; 
			session_pool->Add(session);
		}, 
		[](Session::Pointer session, const ByteArray& byte_array){ // on_receive
			std::cout << "on_receive!!!" << std::endl; 
		},
		[&session_pool](Session::Pointer session){ // on_close
			std::cout << "on_close!!!" << std::endl; 
			session_pool->Erase(session);
		},
		[&service, &session_pool](const ByteArray& byte_array){ //broadcast
			Broadcast(session_pool, byte_array);
			//Send(client, "127.0.0.1", 54321, utl::String2ByteArray("hello"));
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

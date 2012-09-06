#ifdef PEER_UNIT_TEST
#include "Peer.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::ntw;

auto PeerTestCuiApp(boost::asio::io_service& service, Peer::Pointer peer, 
	Client::OnConnectFunc on_connect_func, 
	Session::OnReceiveFunc on_receive_func,
	Session::OnCloseFunc on_close_func,
	boost::function<void (const ByteArray&)> broadcast_func,
	boost::function<void ()> close_func) -> void
{
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

				peer->Connect(hostname, port, 
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


auto PeerTestCuiApp(
		boost::asio::io_service& service, Peer::Pointer peer) -> void { 
	auto session_pool = SessionPool::Create();
	PeerTestCuiApp(service, peer, 
		[&session_pool](Session::Pointer session){  // on_connect
			std::cout << "on_connect!!!" << std::endl; 
			session_pool->Add(session);
		}, 
		[](Session::Pointer session, const ByteArray& byte_array){ // on_receive
			std::cout << "on_receive!!!" << std::endl; 
		},
		[&session_pool](Session::Pointer session){ // on_close
			session_pool->Erase(session);
		},
		[&session_pool](const ByteArray& byte_array){ //broadcast
			Broadcast(session_pool, byte_array);
		},
		[&session_pool](){ // close 
			std::cout << "close" << std::endl; 
			for(auto& session : *session_pool){
				session->Close();	
			}
		}
	);
}

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
	auto peer = Peer::Create(service, server_port, buffer_size, std::cout);
	SetCallbacks(peer,
		[&session_pool](Session::Pointer session){
			std::cout << "on_accept_func called:" << std::endl;
			session_pool->Add(session);
		},
		[](Session::Pointer session, const ByteArray& byte_array){ 
			std::cout << "on receive from accepted session:" 
				<< utl::ByteArray2String(byte_array) << std::endl;
		},
		[&session_pool](Session::Pointer session){
			session_pool->Erase(session);
		}
	);

	peer->StartAccept();

	std::cout << "accept port is " << server_port << std::endl;
	PeerTestCuiApp(service, peer, 
		[&session_pool](Session::Pointer session){  // on_connect
			std::cout << "on_connect!!!" << std::endl; 
			session_pool->Add(session);
		}, 
		[](Session::Pointer session, const ByteArray& byte_array){ // on_receive
			std::cout << "on_receive!!!" << std::endl; 
		},
		[&session_pool](Session::Pointer session){ // on_close
			session_pool->Erase(session);
		},
		[&service, &peer, &session_pool](const ByteArray& byte_array){ //broadcast
			Broadcast(session_pool, byte_array);
			//Send(peer, "127.0.0.1", 54321, utl::String2ByteArray("hello"));
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

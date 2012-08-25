#pragma once
//P2pCore:20120816
#include <iostream>
#include <vector>
//#include <jsoncpp/json.h>
#include <deque>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Session.h"
#include "SessionPool.h"

namespace nr
{

class P2pCore : public boost::enable_shared_from_this<P2pCore>
{
public:
	using Pointer = boost::shared_ptr<P2pCore>;
	using OnReceiveFunc = boost::function<void (P2pCore::Pointer, Session::Pointer, const utl::ByteArray&)>;
	
	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			OnReceiveFunc from_upper_func,
			OnReceiveFunc from_lower_func) -> Pointer {
		auto created = Pointer(
			new P2pCore(service, port, buffer_size, from_upper_func, from_lower_func));	
		created->StartAccept();
		return created;
	}


	auto Connect(const std::string& hostname, int port) -> void {
		DEBUG_PRINT("connectiong...", 50);

		boost::asio::ip::tcp::resolver resolver(this->service); //名前解決
		auto query = boost::asio::ip::tcp::resolver::query(
			hostname, boost::lexical_cast<std::string>(port));
		auto endpoint_iter = resolver.resolve(query);

		auto new_upper_session = Session::Create(
			this->service, this->upper_session_pool_ptr, 
			boost::bind(this->from_upper_func, shared_from_this(), _1, _2));

		boost::asio::async_connect(
			new_upper_session->GetSocketRef(),
			endpoint_iter,
			boost::bind(
				&P2pCore::OnConnect, shared_from_this(), new_upper_session,
				boost::asio::placeholders::error
			)
		);	
	}

	auto BroadcastToUpper(const utl::ByteArray& byte_array) -> void {
		DEBUG_PRINT("broadcast TO UPPER", 50);
		Broadcast(this->service, this->upper_session_pool_ptr, byte_array);
	}

	auto BroadcastToLower(const utl::ByteArray& byte_array) -> void {
		DEBUG_PRINT("broadcast TO LOWER", 50);
		Broadcast(this->service, this->lower_session_pool_ptr, byte_array);
	}
	
	auto CloseUpperSession(unsigned int index) -> void {
		DEBUG_PRINT("close upper session", 50);
		upper_session_pool_ptr->At(index)->Close();
	}

	auto CloseLowerSession(unsigned int index) -> void {
		DEBUG_PRINT("close upper session", 50);
		lower_session_pool_ptr->At(index)->Close();
	}

	auto GetSessionListStr() -> std::string {
		if(!upper_session_pool_ptr->IsEmpty() || !lower_session_pool_ptr->IsEmpty()){
			std::vector<std::string> integrated;
			integrated.push_back("upper:");
			for(auto& session : *upper_session_pool_ptr){
				integrated.push_back(GetAddressStr(session));
			}

			integrated.push_back("\nlower:");	
			for(auto& session : *lower_session_pool_ptr){
				integrated.push_back(GetAddressStr(session));
			}
			return utl::StrJoin(integrated, " ");
		}
		else{
			return "no session connected.";	
		}
	}

private:
	P2pCore(boost::asio::io_service& service, int port, int buffer_size,
			OnReceiveFunc from_upper_func,
			OnReceiveFunc from_lower_func)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		buffer_size(buffer_size),
		from_upper_func(from_upper_func),
		from_lower_func(from_lower_func),
		upper_session_pool_ptr(SessionPool::Create()),
		lower_session_pool_ptr(SessionPool::Create())
	{}
	
	auto StartAccept() -> void {
		auto new_lower_session = Session::Create(
			this->service, this->lower_session_pool_ptr, 
			boost::bind(this->from_lower_func, shared_from_this(), _1, _2));

		this->acceptor.async_accept(
			new_lower_session->GetSocketRef(),
			boost::bind(
				&P2pCore::OnAccept, shared_from_this(), new_lower_session,
				boost::asio::placeholders::error
			)
		);
	}

	auto OnAccept(
		Session::Pointer session, 
		const boost::system::error_code& error_code
	) 
	-> void {
		if(!error_code){
			std::cout << "accept:" << GetAddressStr(session) << std::endl;
			this->lower_session_pool_ptr->Add(session);
			session->StartReceive();
			std::cout << "lower session_queue size:" 
				<< this->lower_session_pool_ptr->GetSize() << std::endl;
		}
		else{
			std::cout << "accept failure" << std::endl;		
		}
		this->StartAccept();
	}
	
	auto OnConnect(
		Session::Pointer session, 
		const boost::system::error_code& error_code
	) 
	-> void {
		if(!error_code){
			this->upper_session_pool_ptr->Add(session);
			session->StartReceive();
			std::cout << "upper session_queue size:" 
				<< this->upper_session_pool_ptr->GetSize() << std::endl;
		}
		else{
			std::cout << "connect failure." << std::endl;	
		}
	}

	boost::asio::io_service& service;
	boost::asio::ip::tcp::acceptor acceptor;
	int buffer_size;
	OnReceiveFunc from_upper_func;
	OnReceiveFunc from_lower_func;
	SessionPool::Pointer upper_session_pool_ptr;
	SessionPool::Pointer lower_session_pool_ptr;
};

auto P2pCoreTestCuiApp(boost::asio::io_service& service, P2pCore::Pointer core_ptr) -> void
{
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
				if(which != "upper" && which != "lower"){
					std::cout << 
						"invalid.(please input \"upper\" or \"lower\")" << std::endl;	
				}
				else{
					const auto session_index = 
						utl::GetInput<unsigned int>("sesion index?:");
					if(which == "upper"){
						core_ptr->CloseUpperSession(session_index);	
					}
					else{
						core_ptr->CloseLowerSession(session_index);	
					}
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
}

}


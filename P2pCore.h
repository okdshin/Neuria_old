#pragma once
//P2pCore:20120816
#include <iostream>
#include <vector>
//#include <jsoncpp/json.h>
//#include <deque>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Session.h"
#include "SessionPool.h"

namespace nr
{

class P2pCore : public boost::enable_shared_from_this<P2pCore> {
public:
	using Pointer = boost::shared_ptr<P2pCore>;
	using OnAcceptFunc = boost::function<void (Session::Pointer)>;
	using OnConnectFunc = boost::function<void (Session::Pointer)>;
	
	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_func,
			Session::OnCloseFunc on_close_func,
			std::ostream& os) -> Pointer {
		auto created = Pointer(
			new P2pCore(service, port, buffer_size, 
				on_accept_func, on_receive_func, on_close_func, os));	
		created->StartAccept();
		return created;
	}

	auto Connect(const std::string& hostname, int port, 
			OnConnectFunc on_connect_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func) -> void {
		this->os << "connectiong..." << std::endl;

		boost::asio::ip::tcp::resolver resolver(this->service); //名前解決
		auto query = boost::asio::ip::tcp::resolver::query(
			hostname, boost::lexical_cast<std::string>(port));
		auto endpoint_iter = resolver.resolve(query);

		auto new_session = Session::Create(
			this->service, this->buffer_size, on_receive_func, on_close_func, this->os);

		boost::asio::async_connect(
			new_session->GetSocketRef(),
			endpoint_iter,
			boost::bind(
				&P2pCore::OnConnect, shared_from_this(), on_connect_func, new_session,
				boost::asio::placeholders::error
			)
		);	
	}

private:
	P2pCore(boost::asio::io_service& service, int port, int buffer_size,
			OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func,
			std::ostream& os)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		buffer_size(buffer_size), on_accept_func(on_accept_func), 
		on_receive_func(on_receive_func), on_close_func(on_close_func), os(os){}
	
	auto StartAccept() -> void {
		auto new_session = Session::Create(this->service, this->buffer_size,
			this->on_receive_func, this->on_close_func, this->os);

		this->acceptor.async_accept(
			new_session->GetSocketRef(),
			boost::bind(
				&P2pCore::OnAccept, shared_from_this(), on_accept_func, new_session,
				boost::asio::placeholders::error
			)
		);
	}

	auto OnAccept(OnAcceptFunc on_accept_func, Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
			this->os << "accept:" << GetAddressStr(session) << std::endl;
			this->on_accept_func(session);
			session->StartReceive();
		}
		else{
			this->os << "accept failure" << std::endl;		
		}
		this->StartAccept();
	}
	
	auto OnConnect(OnConnectFunc on_connect_func, Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
			this->os << "connect:" << GetAddressStr(session) << std::endl;
			on_connect_func(session);
			session->StartReceive();
		}
		else{
			this->os << "connect failure. " << error_code.message()  << std::endl;	
		}
	}

	boost::asio::io_service& service;
	boost::asio::ip::tcp::acceptor acceptor;
	int buffer_size;
	OnAcceptFunc on_accept_func;
	Session::OnReceiveFunc on_receive_func;
	Session::OnCloseFunc on_close_func;
	std::ostream& os;
};

auto P2pCoreTestCuiApp(boost::asio::io_service& service, P2pCore::Pointer core_ptr, 
	P2pCore::OnConnectFunc on_connect_func, 
	Session::OnReceiveFunc on_receive_func,
	Session::OnCloseFunc on_close_func,
	boost::function<void (const utl::ByteArray&)> broadcast_func,
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

				core_ptr->Connect(hostname, port, 
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

}


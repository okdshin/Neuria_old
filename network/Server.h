#pragma once
//Server:20120905
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Session.h"
#include "SessionPool.h"
#include "../ByteArray.h"

namespace nr{
namespace ntw{

class Server : public boost::enable_shared_from_this<Server> {
public:
	using Pointer = boost::shared_ptr<Server>;
	using OnAcceptFunc = boost::function<void (Session::Pointer)>;

	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_func,
			Session::OnCloseFunc on_close_func,
			std::ostream& os) -> Pointer {
		return  Pointer(new Server(service, port, buffer_size, 
				on_accept_func, on_receive_func, on_close_func, os));	
	}
	
	auto StartAccept() -> void {
		auto new_session = Session::Create(this->service, this->buffer_size,
			this->on_receive_func, this->on_close_func, this->os);

		this->acceptor.async_accept(
			new_session->GetSocketRef(),
			boost::bind(
				&Server::OnAccept, this->shared_from_this(), on_accept_func, new_session,
				boost::asio::placeholders::error
			)
		);
	}

private:
    Server(boost::asio::io_service& service, int port, int buffer_size,
			OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func,
			std::ostream& os)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v4(), port)),
		buffer_size(buffer_size), on_accept_func(on_accept_func), 
		on_receive_func(on_receive_func), on_close_func(on_close_func), os(os){}
	

	auto OnAccept(OnAcceptFunc on_accept_func, Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
			this->os << "accept:" << GetRemoteAddressStr(session) << ":"
				<< GetRemotePort(session) << std::endl;
			this->on_accept_func(session);
			session->StartReceive();
		}
		else{
			this->os << "accept failure" << std::endl;		
		}
		this->StartAccept();
	}

	boost::asio::io_service& service;
	boost::asio::ip::tcp::acceptor acceptor;
	int buffer_size;
	OnAcceptFunc on_accept_func;
	Session::OnReceiveFunc on_receive_func;
	Session::OnCloseFunc on_close_func;
	std::ostream& os;

};

}
}

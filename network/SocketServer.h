#pragma once
//SocketServer:20120905
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "SocketSession.h"
#include "SessionPool.h"
#include "Server.h"
#include "../ByteArray.h"

namespace nr{
namespace ntw{

class SocketServer : public Server, public boost::enable_shared_from_this<SocketServer> {
public:
	using Pointer = boost::shared_ptr<SocketServer>;

	static auto Create(boost::asio::io_service& service, 
			int port, int buffer_size, std::ostream& os) -> Pointer {
		return  Pointer(new SocketServer(service, port, buffer_size, os));	
	}


	auto StartAccept() -> void {
		auto new_session = SocketSession::Create(this->service, this->buffer_size,
			this->on_receive_func, this->on_close_func, this->os);

		this->acceptor.async_accept(
			new_session->GetSocketRef(),
			boost::bind(
				&SocketServer::OnAccept, this->shared_from_this(), new_session,
				boost::asio::placeholders::error
			)
		);
	}

private:
    SocketServer(boost::asio::io_service& service, 
			int port, int buffer_size, std::ostream& os)
		: service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(
			boost::asio::ip::tcp::v4(), port)), buffer_size(buffer_size), 
		on_accept_func([](Session::Pointer){}), 
		on_receive_func([](Session::Pointer, const ByteArray&){}), 
		on_close_func([](Session::Pointer){}), os(os){}
	
	auto DoSetOnReceiveFunc(Session::OnReceiveFunc on_receive_func) -> void {
		this->on_receive_func = on_receive_func;
	}

	auto DoSetOnAcceptFunc(SocketServer::OnAcceptFunc on_accept_func) -> void {
		this->on_accept_func = on_accept_func;
	}
	
	auto DoSetOnCloseFunc(Session::OnCloseFunc on_close_func) -> void {
		this->on_close_func = on_close_func;
	}

	auto OnAccept(Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
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
	SocketServer::OnAcceptFunc on_accept_func;
	SocketSession::OnReceiveFunc on_receive_func;
	SocketSession::OnCloseFunc on_close_func;
	std::ostream& os;

};



}
}

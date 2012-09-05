#pragma once
//Peer:20120816
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

class Peer : public boost::enable_shared_from_this<Peer> {
public:
	using Pointer = boost::shared_ptr<Peer>;
	using OnAcceptFunc = boost::function<void (Session::Pointer)>;
	using OnConnectFunc = boost::function<void (Session::Pointer)>;
	
	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_func,
			Session::OnCloseFunc on_close_func,
			std::ostream& os) -> Pointer {
		auto created = Pointer(
			new Peer(service, port, buffer_size, 
				on_accept_func, on_receive_func, on_close_func, os));	
		created->StartAccept();
		return created;
	}

	auto Connect(const std::string& hostname, int port, 
			OnConnectFunc on_connect_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func) -> void {
		this->os << "connectiong..." << std::endl;

		boost::asio::ip::tcp::resolver resolver(this->service); //name resolving
		auto query = boost::asio::ip::tcp::resolver::query(
			hostname, boost::lexical_cast<std::string>(port));
		auto endpoint_iter = resolver.resolve(query);

		auto new_session = Session::Create(
			this->service, this->buffer_size, on_receive_func, on_close_func, this->os);

		boost::asio::async_connect(
			new_session->GetSocketRef(), endpoint_iter, boost::bind(
				&Peer::OnConnect, this->shared_from_this(), 
				on_connect_func, new_session, boost::asio::placeholders::error));	
	}

private:
	Peer(boost::asio::io_service& service, int port, int buffer_size,
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
				&Peer::OnAccept, this->shared_from_this(), on_accept_func, new_session,
				boost::asio::placeholders::error
			)
		);
	}

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
	
	auto OnConnect(OnConnectFunc on_connect_func, Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
			this->os << "connect:" << GetRemoteAddressStr(session) << ":" 
				<< GetRemotePort(session) << std::endl;
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

auto Send(Peer::Pointer core_ptr, const std::string& address, int port, 
		const ByteArray& byte_array) -> void {
	core_ptr->Connect(address, port, 
		[byte_array](Session::Pointer session){
			session->Send(byte_array);
			session->Close();
		},
		[](Session::Pointer, const ByteArray&){},
		[](Session::Pointer){});
}


}
}


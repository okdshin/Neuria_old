#pragma once
//Connector:20120905
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

class Connector : public boost::enable_shared_from_this<Connector> {
public:
	using Pointer = boost::shared_ptr<Connector>;
	using OnConnectFunc = boost::function<void (Session::Pointer)>;
	
	static auto Create(boost::asio::io_service& service, 
			int buffer_size, std::ostream& os) -> Pointer {
		return Pointer(new Connector(service, buffer_size, os));	
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

	
		this->os << "query resolved:"<< endpoint_iter->endpoint().address().to_string() 
			<< ":" <<  endpoint_iter->endpoint().port() << std::endl;
		
		boost::asio::async_connect(
			new_session->GetSocketRef(), endpoint_iter, boost::bind(
				&Connector::OnConnect, this->shared_from_this(), 
				on_connect_func, new_session, boost::asio::placeholders::error));	
	}

private:
    Connector(boost::asio::io_service& service, int buffer_size, std::ostream& os)
		: service(service), buffer_size(buffer_size), os(os){}
	
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
	int buffer_size;
	std::ostream& os;
};

}
}


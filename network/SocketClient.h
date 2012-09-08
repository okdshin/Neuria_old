#pragma once
//SocketClient:20120905
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include "SocketSession.h"
#include "SessionPool.h"
#include "Client.h"
#include "../ByteArray.h"
#include "../NodeId.h"

namespace nr{
namespace ntw{

class SocketClient : public Client, public boost::enable_shared_from_this<SocketClient> {
public:
	using Pointer = boost::shared_ptr<SocketClient>;
	
	static auto Create(boost::asio::io_service& service, 
			int buffer_size, std::ostream& os) -> Pointer {
		return Pointer(new SocketClient(service, buffer_size, os));	
	}

private:
    SocketClient(boost::asio::io_service& service, int buffer_size, std::ostream& os)
		: service(service), buffer_size(buffer_size), os(os){}
	
	auto DoConnect(const NodeId& node_id, 
			OnConnectFunc on_connect_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func) -> void {
		this->os << "connectiong..." << std::endl;

		boost::asio::ip::tcp::resolver resolver(this->service); //name resolving

		auto parsed_id = std::vector<std::string>();
		boost::split(parsed_id, node_id, boost::is_any_of("/"));
		assert(parsed_id.size() == 2);
		auto query = boost::asio::ip::tcp::resolver::query(
			parsed_id.at(0), parsed_id.at(1));
		
		auto endpoint_iter = resolver.resolve(query);

		auto new_session = SocketSession::Create(
			this->service, this->buffer_size, on_receive_func, on_close_func, 
			this->os);
	
		this->os << "query resolved:"
			<< endpoint_iter->endpoint().address().to_string() 
			<< ":" <<  endpoint_iter->endpoint().port() << std::endl;
		
		boost::asio::async_connect(
			new_session->GetSocketRef(), endpoint_iter, boost::bind(
				&SocketClient::OnConnect, this->shared_from_this(), 
				on_connect_func, new_session, boost::asio::placeholders::error));	
	}

	auto OnConnect(OnConnectFunc on_connect_func, Session::Pointer session, 
			const boost::system::error_code& error_code) -> void {
		if(!error_code){
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


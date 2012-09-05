#pragma once
//Peer:20120816
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Session.h"
#include "SessionPool.h"
#include "Server.h"
#include "Client.h"
#include "../ByteArray.h"

namespace nr{
namespace ntw{

class Peer : public boost::enable_shared_from_this<Peer> {
public:
	using Pointer = boost::shared_ptr<Peer>;
	
	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			Server::OnAcceptFunc on_accept_func, 
			Session::OnReceiveFunc on_receive_accepted_func,
			Session::OnCloseFunc on_close_accepted_func,
			std::ostream& os) -> Pointer {
		auto server = Server::Create(service, port, buffer_size, 
			on_accept_func, on_receive_accepted_func, on_close_accepted_func, os);
		auto client = Client::Create(service, buffer_size, os);
		return Pointer(new Peer(server, client, os));	
	}

	auto Connect(const std::string& hostname, int port, 
			Client::OnConnectFunc on_connect_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func) -> void {
		this->os << "connectiong..." << std::endl;
		this->client->Connect(hostname, port, 
			on_connect_func, on_receive_func, on_close_func);
	}

	auto StartAccept() -> void {
		this->server->StartAccept();
	}

private:
	Peer(Server::Pointer server, Client::Pointer client, std::ostream& os)
		: server(server), client(client), os(os){}


	Server::Pointer server;
	Client::Pointer client;
	std::ostream& os;
};

auto Send(Peer::Pointer peer, const std::string& address, int port, 
		const ByteArray& byte_array) -> void {
	peer->Connect(address, port, 
		[byte_array](Session::Pointer session){
			session->Send(byte_array);
			session->Close();
		},
		[](Session::Pointer, const ByteArray&){},
		[](Session::Pointer){});
}


}
}


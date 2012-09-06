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

class Peer : public ServerBase, public boost::enable_shared_from_this<Peer> {
public:
	using Pointer = boost::shared_ptr<Peer>;
	
	static auto Create(boost::asio::io_service& service, 
			int port, int buffer_size, std::ostream& os) -> Pointer {
		return Pointer(new Peer(
			Server::Create(service, port, buffer_size, os), 
			Client::Create(service, buffer_size, os), os));
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
	
	auto DoSetOnReceiveFunc(Session::OnReceiveFunc on_receive_func) -> void {
		this->server->SetOnReceiveFunc(on_receive_func);	
	}

	auto DoSetOnAcceptFunc(Server::OnAcceptFunc on_accept_func) -> void {
		this->server->SetOnAcceptFunc(on_accept_func);	
	}
	
	auto DoSetOnCloseFunc(Session::OnCloseFunc on_close_func) -> void {
		this->server->SetOnCloseFunc(on_close_func);	
	}

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


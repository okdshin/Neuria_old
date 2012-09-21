#pragma once
//Client:20120908
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "../NodeId.h"
#include "Session.h"
#include "SessionPool.h"

namespace nr{
namespace ntw{

class Client{
public:
	using Pointer = boost::shared_ptr<Client>;
	using OnConnectFunc = boost::function<void (Session::Pointer)>;

	virtual auto Connect(const NodeId& node_id, 
			OnConnectFunc on_connect_func, 
			Session::OnReceiveFunc on_receive_func, 
			Session::OnCloseFunc on_close_func) -> void {
		this->DoConnect(node_id, on_connect_func, on_receive_func, on_close_func);		
	}

    virtual ~Client(){}
	
private:
	
	virtual auto DoConnect(const NodeId& node_id, OnConnectFunc on_connect_func, 
		Session::OnReceiveFunc on_receive_func, Session::OnCloseFunc) -> void = 0;
};

auto Connect(Client::Pointer client, const NodeId& node_id, SessionPool::Pointer pool, 
		Session::OnReceiveFunc on_receive_func) -> void {
	client->Connect(node_id, 
		[pool](Session::Pointer session){ pool->Add(session); }, 
		on_receive_func, 
		[pool](Session::Pointer session){ pool->Erase(session); });	
}

auto Communicate(Client::Pointer client, const NodeId& node_id, 
		const ByteArray& byte_array, Session::OnReceiveFunc on_receive_func) -> void {
	client->Connect(node_id, 
		[byte_array](Session::Pointer session){ session->Send(byte_array); }, 
		on_receive_func,
		[](Session::Pointer){});	
}

auto Send(Client::Pointer client, const NodeId& node_id, 
		const ByteArray& byte_array) -> void {
	client->Connect(node_id,
		[byte_array](Session::Pointer session){ session->Send(byte_array); }, 
		[](Session::Pointer, const ByteArray&){}, 
		[](Session::Pointer){});				
}

}
}


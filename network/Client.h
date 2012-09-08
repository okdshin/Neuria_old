#pragma once
//Client:20120908
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include "../NodeId.h"
#include "Session.h"

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

}
}


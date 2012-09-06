#pragma once
//ServerBase:20120906
#include <iostream>
#include "Session.h"

namespace nr{
namespace ntw{

class ServerBase {
public:
	using Pointer = boost::shared_ptr<ServerBase>;
	using OnAcceptFunc = boost::function<void (Session::Pointer)>;
	
	virtual auto SetOnReceiveFunc(Session::OnReceiveFunc on_receive_func) -> void = 0;
	virtual auto SetOnAcceptFunc(OnAcceptFunc on_accept_func) -> void = 0;
	virtual auto SetOnCloseFunc(Session::OnCloseFunc on_close_func) -> void = 0;
	
	virtual auto StartAccept() -> void = 0;
};

auto SetCallbacks(ServerBase::Pointer target, 
		Server::OnAcceptFunc on_accept, 
		Session::OnReceiveFunc on_receive, 
		Session::OnCloseFunc on_close) -> void {
	target->SetOnAcceptFunc(on_accept);
	target->SetOnReceiveFunc(on_receive);
	target->SetOnCloseFunc(on_close);
}

}
}


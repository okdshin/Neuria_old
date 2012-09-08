#pragma once
//Server:20120906
#include <iostream>
#include "Session.h"

namespace nr{
namespace ntw{

class Server {
public:
	using Pointer = boost::shared_ptr<Server>;
	using OnAcceptFunc = boost::function<void (Session::Pointer)>;

	auto SetOnReceiveFunc(Session::OnReceiveFunc on_receive_func) -> void {
		this->DoSetOnReceiveFunc(on_receive_func);	
	}
	
	auto SetOnAcceptFunc(OnAcceptFunc on_accept_func) -> void {
		this->DoSetOnAcceptFunc(on_accept_func);	
	}
	
	auto SetOnCloseFunc(Session::OnCloseFunc on_close_func) -> void {
		this->DoSetOnCloseFunc(on_close_func);	
	}

private:
	virtual auto DoSetOnReceiveFunc(Session::OnReceiveFunc on_receive_func) -> void = 0;
	virtual auto DoSetOnAcceptFunc(OnAcceptFunc on_accept_func) -> void = 0;
	virtual auto DoSetOnCloseFunc(Session::OnCloseFunc on_close_func) -> void = 0;
	
	virtual auto StartAccept() -> void = 0;
};

auto SetCallbacks(Server::Pointer target, 
		Server::OnAcceptFunc on_accept, 
		Session::OnReceiveFunc on_receive, 
		Session::OnCloseFunc on_close) -> void {
	target->SetOnAcceptFunc(on_accept);
	target->SetOnReceiveFunc(on_receive);
	target->SetOnCloseFunc(on_close);
}

}
}


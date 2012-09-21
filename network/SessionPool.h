#pragma once
//SessionPool:20120822
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "Session.h"

namespace nr{
namespace ntw{

class SessionPool : public boost::enable_shared_from_this<SessionPool>{
public:
	using Pointer = boost::shared_ptr<SessionPool>;
	static auto Create() -> Pointer {
		return Pointer(new SessionPool());	
	}

	auto Add(Session::Pointer session) -> void {
		this->sessions.push_back(session);	
	}

	auto Erase(Session::Pointer session) -> void { // not close session.
		std::cout << "session erase from pool" << std::endl;
		sessions.erase(std::find(sessions.begin(), sessions.end(), session));
	}

	auto At(unsigned int index) -> Session::Pointer {
		return this->sessions.at(index);	
	}

	auto GetSize() -> unsigned int {
		return this->sessions.size();
	}

	auto IsEmpty() -> bool {
		return this->sessions.empty();	
	}

	auto Get(const NodeId& node_id) -> Session::Pointer{
		auto iter = std::find_if(this->sessions.begin(), this->sessions.end(), 
			[&node_id](Session::Pointer session){ 
				return session->GetNodeId() == node_id; });	
		assert(iter != this->sessions.end());
		return *iter;
	}

	//range-based-forで使われることを想定している。
	using iterator = std::vector<Session::Pointer>::iterator;
	auto begin() -> iterator {
		return this->sessions.begin();
	}
	
	auto end() -> iterator {
		return this->sessions.end();
	}

private:
	SessionPool():sessions(){}
	std::vector<Session::Pointer> sessions;
};

auto Broadcast(
		SessionPool::Pointer session_pool, const ByteArray& byte_array) -> void {	
	if(!session_pool->IsEmpty()){
		for(auto& session : *session_pool){
			session->Send(byte_array);
		}
	}
	else{
		std::cout << "no peer. broadcast failed." << std::endl;	
	}
}

}
}

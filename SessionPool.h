#pragma once
//SessionPool:20120822
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "Utility.h"
#include "SessionBase.h"

namespace nr
{
class SessionPool {
public:
	using Pointer = boost::shared_ptr<SessionPool>;
	static auto Create() -> Pointer {
		return Pointer(new SessionPool());	
	}

	auto Add(SessionBase::Pointer session) -> void {
		this->sessions.push_back(session);	
	}

	auto Erase(SessionBase::Pointer session) -> void {
		std::cout << "session erase from pool" << std::endl;
		sessions.erase(std::find(sessions.begin(), sessions.end(), session));
	}

	auto At(unsigned int index) -> SessionBase::Pointer {
		return this->sessions.at(index);	
	}

	auto GetSize() -> unsigned int {
		return this->sessions.size();
	}

	auto IsEmpty() -> bool {
		return this->sessions.empty();	
	}

	//range-based-forで使われることを想定している。
	using Iterator = std::vector<SessionBase::Pointer>::iterator;
	auto begin() -> Iterator {
		return this->sessions.begin();
	}
	
	auto end() -> Iterator {
		return this->sessions.end();
	}

private:
	SessionPool():sessions(){}
	std::vector<SessionBase::Pointer> sessions;
};

auto Broadcast(boost::asio::io_service& service, SessionPool::Pointer session_pool_ptr, 
		const utl::ByteArray& byte_array) -> void {	
	if(!session_pool_ptr->IsEmpty()){
		for(auto& session : *session_pool_ptr){
			service.post(
				boost::bind(&SessionBase::Send, session, byte_array));
		}
	}
	else{
		std::cout << "no peer. broadcast failed." << std::endl;	
	}
}



}


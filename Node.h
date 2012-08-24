#pragma once
//Node:20120825
#include <iostream>
#include "P2pCore.h"

namespace nr
{
class Node{
public:
	using OnReceiveFunc = 
		boost::function<void (P2pCore::Pointer, Session::Pointer, const utl::ByteArray)>;

	Node(boost::asio::io_service& io_service, int port, 
			OnReceiveFunc from_upper_func, 
			OnReceiveFunc from_lower_func)
		:core_ptr(P2pCore::Create(io_service, port, 
			boost::bind(&Node::OnReceiveFromUpperFunc, this, _1, _2),
			boost::bind(&Node::OnReceiveFromLowerFunc, this, _1, _2))),
		from_upper_func(from_upper_func),
		from_lower_func(from_lower_func){}

	auto GetCorePtr() -> P2pCore::Pointer
	{
		return this->core_ptr;	
	}
private:
	auto OnReceiveFromUpperFunc(Session::Pointer session, 
			const utl::ByteArray& received_byte_array) -> void {
		this->from_upper_func(this->core_ptr, session, received_byte_array);	
	}
	
	auto OnReceiveFromLowerFunc(Session::Pointer session, 
			const utl::ByteArray& received_byte_array) -> void {
		this->from_lower_func(this->core_ptr, session, received_byte_array);
	}

	P2pCore::Pointer core_ptr;
	OnReceiveFunc from_upper_func;
	OnReceiveFunc from_lower_func;

};
}


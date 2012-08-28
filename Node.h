#pragma once
//Node:20120825
#include <iostream>
#include <map>
#include "P2pCore.h"

namespace nr
{
template<class Key>
class Node{
public:
	using Pointer = boost::shared_ptr<Node<Key>>;
	using KeyPickupper = boost::function<Key (const utl::ByteArray&)>;
	using FuncDict = std::map<Key, P2pCore::OnReceiveFunc>;

	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			KeyPickupper key_pickupper, 
			const FuncDict& for_upper_func_dict, 
			const FuncDict& for_lower_func_dict, 
			std::ostream& os) -> Pointer {
		return Pointer(new Node(service, port, buffer_size, 
				key_pickupper, for_upper_func_dict, for_lower_func_dict, os));
	}

	auto CallMatchFuncForUpper(P2pCore::Pointer core, 
			Session::Pointer session, const utl::ByteArray& byte_array) -> void {
		CallMatchFunc(this->for_upper_func_dict, core, session, byte_array);
	}
		
	auto CallMatchFuncForLower(P2pCore::Pointer core, 
			Session::Pointer session, const utl::ByteArray& byte_array) -> void {
		CallMatchFunc(this->for_lower_func_dict, core, session, byte_array);
	}

	auto GetCorePtr() -> P2pCore::Pointer {
		return this->core_ptr;	
	}

private:
	Node(boost::asio::io_service& service, int port, int buffer_size, 
			KeyPickupper key_pickupper, 
			const FuncDict& for_upper_func_dict, 
			const FuncDict& for_lower_func_dict, 
			std::ostream& os)
		:service(service), key_pickupper(key_pickupper),
		for_upper_func_dict(for_upper_func_dict),
		for_lower_func_dict(for_lower_func_dict),
		core_ptr(P2pCore::Create(service, port, buffer_size, 
			boost::bind(&Node::CallMatchFuncForUpper, this, _1, _2, _3), 
			boost::bind(&Node::CallMatchFuncForLower, this, _1, _2, _3),
			os)){}

	auto CallMatchFunc(FuncDict& func_dict, P2pCore::Pointer core, 
			Session::Pointer session, const utl::ByteArray& byte_array) -> void {
		this->service.post(boost::bind(
			func_dict[this->key_pickupper(byte_array)], core, session, byte_array));
	}

	boost::asio::io_service& service;
	KeyPickupper key_pickupper;
	FuncDict for_upper_func_dict;
	FuncDict for_lower_func_dict;
	P2pCore::Pointer core_ptr;

};
}


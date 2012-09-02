#pragma once
//FuncController:20120902
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "Utility.h"
#include "Session.h"

namespace nr
{
template<class Key>
class FuncController{
public:
	using Pointer = boost::shared_ptr<FuncController>;
	using KeyPickupper = boost::function<Key (const utl::ByteArray&)>;
	using FuncDict = std::map<Key, Session::OnReceiveFunc>;

	static auto Create(
			boost::asio::io_service& service, KeyPickupper key_pickupper) -> Pointer {
		return Pointer(new FuncController(service, key_pickupper));
	}

	auto Register(const Key& key, Session::OnReceiveFunc func) -> void {
		this->func_dict[key] = func;	
	}

	auto CallMatchFunc(
			Session::Pointer session, const utl::ByteArray& byte_array) -> void {
		this->service.post(boost::bind(
			this->func_dict[this->key_pickupper(byte_array)], session, byte_array));
	}
    
private:
	FuncController(boost::asio::io_service& service, KeyPickupper key_pickupper)
		: service(service), key_pickupper(key_pickupper), func_dict(){}

	boost::asio::io_service& service;
	KeyPickupper key_pickupper;
	FuncDict func_dict;
};
}


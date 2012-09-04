#pragma once
//MatchFuncCaller:20120902
#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "../ByteArray.h"

namespace nr{
namespace utl{

template<class Key>
class MatchFuncCaller{
public:
	using Pointer = boost::shared_ptr<MatchFuncCaller>;
	using KeyPickupper = boost::function<Key (const ByteArray&)>;
	using Func = boost::function<void (const ByteArray&)>;
	using FuncDict = std::map<Key, Func>;

	static auto Create(
			boost::asio::io_service& service, KeyPickupper key_pickupper) -> Pointer {
		return Pointer(new MatchFuncCaller(service, key_pickupper));
	}

	auto Register(const Key& key, Func func) -> void {
		this->func_dict[key] = func;	
	}

	auto CallMatchFunc(const ByteArray& byte_array) -> void {
		this->service.post(boost::bind(
			this->func_dict[this->key_pickupper(byte_array)], byte_array));
	}
    
private:
	MatchFuncCaller(boost::asio::io_service& service, KeyPickupper key_pickupper)
		: service(service), key_pickupper(key_pickupper), func_dict(){}

	boost::asio::io_service& service;
	KeyPickupper key_pickupper;
	FuncDict func_dict;
};

}
}

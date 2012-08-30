#pragma once
//FuncManager:20120825
#include <iostream>
#include <map>
#include <boost/enable_shared_from_this.hpp>
#include "P2pCore.h"

namespace nr
{
template<class Key>
class FuncManager : public boost::enable_shared_from_this<FuncManager<Key>>{
public:
	using Pointer = boost::shared_ptr<FuncManager<Key>>;
	using KeyPickupper = boost::function<Key (const utl::ByteArray&)>;
	using FuncDict = std::map<Key, Session::OnReceiveFunc>;

	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			KeyPickupper key_pickupper, std::ostream& os) -> Pointer {
		auto func_manager_ptr = Pointer(
			new FuncManager(service, key_pickupper, os));
		auto core_ptr = P2pCore::Create(service, port, buffer_size, 
			boost::bind(&FuncManager::AddToPool, func_manager_ptr, _1),
			boost::bind(&FuncManager::CallMatchFunc, func_manager_ptr, _1, _2), 
			boost::bind(&FuncManager::EraseFromPool, func_manager_ptr, _1), os);
		func_manager_ptr->SetCorePtr(core_ptr);
		return func_manager_ptr;
	}

	auto Register(const Key& key, Session::OnReceiveFunc func) -> void {
		this->func_dict[key] = func;
	}

	auto GetCorePtr() -> P2pCore::Pointer {
		return this->core_ptr;	
	}

private:
	FuncManager(
			boost::asio::io_service& service, KeyPickupper key_pickupper, std::ostream& os)
		:service(service), key_pickupper(key_pickupper), func_dict(), core_ptr(), 
		pool_ptr(SessionPool::Create()), os(os){}

	auto SetCorePtr(P2pCore::Pointer core_ptr) -> void {
		this->core_ptr = core_ptr;
	}

	auto AddToPool(Session::Pointer session) -> void {
		this->pool_ptr->Add(session);
	}

	auto EraseFromPool(Session::Pointer session) -> void {
		this->pool_ptr->Erase(session);	
	}

	auto CallMatchFunc(
		Session::Pointer session, const utl::ByteArray& byte_array) -> void {
		this->service.post(boost::bind(
			this->func_dict[this->key_pickupper(byte_array)], session, byte_array));
	}

	boost::asio::io_service& service;
	KeyPickupper key_pickupper;
	FuncDict func_dict;
	P2pCore::Pointer core_ptr;
	SessionPool::Pointer pool_ptr;
	std::ostream& os;
};

}


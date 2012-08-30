#pragma once
//AcceptedManager:20120825
#include <iostream>
#include <map>
#include <boost/enable_shared_from_this.hpp>
#include "P2pCore.h"

namespace nr
{
template<class Key>
class AcceptedManager : public boost::enable_shared_from_this<AcceptedManager<Key>>{
public:
	using Pointer = boost::shared_ptr<AcceptedManager<Key>>;
	using KeyPickupper = boost::function<Key (const utl::ByteArray&)>;
	using FuncDict = std::map<Key, Session::OnReceiveFunc>;

	static auto Create(boost::asio::io_service& service, int port, int buffer_size, 
			SessionPool::Pointer pool_ptr, KeyPickupper key_pickupper, 
			std::ostream& os) -> Pointer {
		auto accepted_manager_ptr = Pointer(
			new AcceptedManager(service, pool_ptr, key_pickupper, os));
		auto core_ptr = P2pCore::Create(service, port, buffer_size, 
			boost::bind(&AcceptedManager::AddToPool, accepted_manager_ptr, _1),
			boost::bind(&AcceptedManager::CallMatchFunc, accepted_manager_ptr, _1, _2), 
			boost::bind(&AcceptedManager::EraseFromPool, accepted_manager_ptr, _1), os);
		accepted_manager_ptr->SetCorePtr(core_ptr);
		return accepted_manager_ptr;
	}

	auto Register(const Key& key, Session::OnReceiveFunc func) -> void {
		this->func_dict[key] = func;
	}

	auto GetCorePtr() -> P2pCore::Pointer {
		return this->core_ptr;	
	}

private:
	AcceptedManager(
			boost::asio::io_service& service, SessionPool::Pointer pool_ptr, 
			KeyPickupper key_pickupper, std::ostream& os)
		:service(service), pool_ptr(pool_ptr), key_pickupper(key_pickupper), func_dict(), 
		core_ptr(), os(os){}

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
	SessionPool::Pointer pool_ptr;
	KeyPickupper key_pickupper;
	FuncDict func_dict;
	P2pCore::Pointer core_ptr;
	std::ostream& os;
};

}


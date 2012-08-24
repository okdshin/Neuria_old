#pragma once
//P2pCore:20120816
#include <iostream>
#include <vector>
//#include <jsoncpp/json.h>
#include <deque>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "Session.h"
#include "SessionPool.h"

namespace nr
{

class P2pCore
{
public:
	using Pointer = boost::shared_ptr<P2pCore>;
	using OnReceiveFunc = boost::function<void (Session::Pointer, const utl::ByteArray&)>;
	
	static auto Create(boost::asio::io_service& service, int port, 
			OnReceiveFunc from_upper_func,
			OnReceiveFunc from_lower_func) -> Pointer {
		return Pointer(new P2pCore(service, port, from_upper_func, from_lower_func));	
	}

	auto StartAccept() -> void {
		auto new_lower_session = Session::Create(
			this->service, this->lower_session_pool_ptr, this->from_lower_func);

		this->acceptor.async_accept(
			new_lower_session->GetSocketRef(),
			boost::bind(
				&P2pCore::HandleAccept, this, new_lower_session,
				boost::asio::placeholders::error
			)
		);
	}

	auto Connect(const std::string& hostname, int port) -> void {
		std::cout << "connecting..." << std::endl;

		//名前解決
		boost::asio::ip::tcp::resolver resolver(this->service);
		auto query = boost::asio::ip::tcp::resolver::query(
			hostname, boost::lexical_cast<std::string>(port));
		auto endpoint_iter = resolver.resolve(query);

		auto new_upper_session = Session::Create(
			this->service, this->upper_session_pool_ptr, this->from_upper_func);
		boost::asio::async_connect(
			new_upper_session->GetSocketRef(),
			endpoint_iter,
			boost::bind(
				&P2pCore::HandleConnect, this, new_upper_session,
				boost::asio::placeholders::error
			)
		);	
	}

	auto BroadcastToUpper(const utl::ByteArray& byte_array) -> void {
		std::cout << "broadcast TO UPPER" << std::endl;

		this->broadcast_byte_array = byte_array; // for refusing linux bad address error.

		if(!upper_session_pool_ptr->IsEmpty()){
			for(auto& session : *upper_session_pool_ptr){
				service.dispatch(boost::bind(&SessionBase::Send, session, byte_array));
			}
		}
		else{
			std::cout << "no peer. broadcast failed." << std::endl;	
		}
	}

	auto BroadcastToLower(const utl::ByteArray& byte_array) -> void {
		std::cout << "broadcast TO LOWER" << std::endl;

		this->broadcast_byte_array = byte_array; // for refusing linux bad address error.

		if(!lower_session_pool_ptr->IsEmpty()){
			for(auto& session : *lower_session_pool_ptr){
				service.dispatch(boost::bind(&SessionBase::Send, session, byte_array));
			}
		}
		else{
			std::cout << "no peer. broadcast failed." << std::endl;	
		}
	}
	
	auto CloseUpperSession(unsigned int index) -> void {
		upper_session_pool_ptr->At(index)->Close();
	}

	auto CloseLowerSession(unsigned int index) -> void {
		lower_session_pool_ptr->At(index)->Close();
	}

	auto GetSessionListStr() -> std::string {
		if(!upper_session_pool_ptr->IsEmpty() || !lower_session_pool_ptr->IsEmpty()){
			std::vector<std::string> integrated;
			integrated.push_back("upper:");
			for(auto& session : *upper_session_pool_ptr){
				integrated.push_back(GetAddressStr(session));
			}

			integrated.push_back("\nlower:");	
			for(auto& session : *lower_session_pool_ptr){
				integrated.push_back(GetAddressStr(session));
			}
			return utl::StrJoin(integrated, " ");
		}
		else{
			return "no session connected.";	
		}
	}

private:
	P2pCore(boost::asio::io_service& service, int port, 
			OnReceiveFunc from_upper_func,
			OnReceiveFunc from_lower_func)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		from_upper_func(from_upper_func),
		from_lower_func(from_lower_func),
		upper_session_pool_ptr(SessionPool::Create()),
		lower_session_pool_ptr(SessionPool::Create())
	{
		this->StartAccept();
	}
	
	auto HandleAccept(
		Session::Pointer session, 
		const boost::system::error_code& error_code
	) 
	-> void {
		if(!error_code){
			std::cout << "accept:" << GetAddressStr(session) << std::endl;
			this->lower_session_pool_ptr->Add(session);
			session->StartReceive();
			std::cout << "lower session_queue size:" 
				<< this->lower_session_pool_ptr->GetSize() << std::endl;
		}
		else{
			std::cout << "accept failure" << std::endl;		
		}
		this->StartAccept();
	}
	
	auto HandleConnect(
		Session::Pointer session, 
		const boost::system::error_code& error_code
	) 
	-> void {
		if(!error_code){
			this->upper_session_pool_ptr->Add(session);
			session->StartReceive();
			std::cout << "upper session_queue size:" 
				<< this->upper_session_pool_ptr->GetSize() << std::endl;
		}
		else{
			std::cout << "connect failure." << std::endl;	
		}
	}

	boost::asio::io_service& service;
	boost::asio::ip::tcp::acceptor acceptor;
	OnReceiveFunc from_upper_func;
	OnReceiveFunc from_lower_func;
	SessionPool::Pointer upper_session_pool_ptr;
	SessionPool::Pointer lower_session_pool_ptr;
	utl::ByteArray broadcast_byte_array;
};

}


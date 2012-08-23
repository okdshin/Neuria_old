#pragma once
//P2pNodeCore:20120816
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

class P2pNodeCore
{
public:
	using OnReceiveFunc = boost::function<void (Session::Pointer, const utl::ByteArray&)>;

	P2pNodeCore(boost::asio::io_service& service, int port, OnReceiveFunc on_receive_func)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		on_receive_func(on_receive_func),
		session_pool_ptr(SessionPool::Create())
	{
		this->StartAccept();
	}

	auto StartAccept() -> void {
		auto new_session = this->CreateSession();

		this->acceptor.async_accept(
			new_session->GetSocketRef(),
			boost::bind(
				&P2pNodeCore::HandleAccept, this, new_session,
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

		auto new_session = this->CreateSession();
		boost::asio::async_connect(
			new_session->GetSocketRef(),
			endpoint_iter,
			boost::bind(
				&P2pNodeCore::HandleConnect, this, new_session,
				boost::asio::placeholders::error
			)
		);	
	}

	auto Broadcast(const utl::ByteArray& byte_array) -> void {
		std::cout << "broadcast" << std::endl;
		this->broadcast_byte_array = byte_array; // for refusing linux bad address error.

		if(!session_pool_ptr->IsEmpty()){
			for(auto& session : *session_pool_ptr){
				service.dispatch(boost::bind(&SessionBase::Send, session, byte_array));
			}
		}
		else{
			std::cout << "no peer. broadcast failed." << std::endl;	
		}
	}

	auto CloseSession(unsigned int index) -> void {
		session_pool_ptr->At(index)->Close();
	}

	auto GetSessionListStr() -> std::string {
		if(!session_pool_ptr->IsEmpty()){
			std::vector<std::string> integrated;
			for(auto& session : *session_pool_ptr){
				integrated.push_back(GetAddressStr(session));
			}
			return utl::StrJoin(integrated, " ");
		}
		else{
			return "no session connected.";	
		}
	}

private:
	auto CreateSession() -> Session::Pointer
	{
		return Session::Create(
			this->service, this->session_pool_ptr, this->on_receive_func);
	}

	auto HandleAccept(
		Session::Pointer session, 
		const boost::system::error_code& error_code
	) 
	-> void {
		if(!error_code){
			std::cout << "accept:" << GetAddressStr(session) << std::endl;
			session_pool_ptr->Add(session);
			session->StartReceive();
			std::cout << "session_queue size:" 
				<< this->session_pool_ptr->GetSize() << std::endl;
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
			this->session_pool_ptr->Add(session);
			session->StartReceive();
		}
		else{
			std::cout << "connect failure." << std::endl;	
		}
	}

	boost::asio::io_service& service;
	boost::asio::ip::tcp::acceptor acceptor;
	OnReceiveFunc on_receive_func;
	SessionPool::Pointer session_pool_ptr;
	utl::ByteArray broadcast_byte_array;
};

}


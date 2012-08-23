#pragma once
//P2pNode:20120816
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

class P2pNode
{
public:
	P2pNode(boost::asio::io_service& service, int port)
		:service(service), 
		acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		session_pool_ptr(SessionPool::Create())
	{
		this->StartAccept();
	}

	auto StartAccept() -> void {
		auto new_session = Session::Create(this->service, this->session_pool_ptr);
		this->acceptor.async_accept(
			new_session->GetSocketRef(),
			boost::bind(
				&P2pNode::HandleAccept, this, new_session,
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

		auto new_session = Session::Create(this->service, this->session_pool_ptr);
		boost::asio::async_connect(
			new_session->GetSocketRef(),
			endpoint_iter,
			boost::bind(
				&P2pNode::HandleConnect, this, new_session,
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
		//service.dispatch(boost::bind(&Session::Close, session_queue.at(index)));
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
		//std::cout << "handle_connect" << std::endl;
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
	SessionPool::Pointer session_pool_ptr;
	utl::ByteArray broadcast_byte_array;
};

}


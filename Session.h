#pragma once
//Session:20120816
#include <iostream>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Utility.h"
#include "SessionPool.h"

namespace nr
{

class Session : public SessionBase, public boost::enable_shared_from_this<Session>
{
public:
	using Pointer = boost::shared_ptr<Session>;
	
	static auto Create(
		boost::asio::io_service& service, SessionPool::Pointer pool) -> Session::Pointer 
	{
		return boost::shared_ptr<Session>(new Session(service, pool));
	}

	auto GetSocketRef() -> boost::asio::ip::tcp::socket& {
		return this->sock;
	}

	auto StartReceive() -> void {
		std::cout << "start receive" << std::endl;
		DoHandleReceive();
	}

	auto Send(const utl::ByteArray& byte_array) -> void {	
		std::cout << "send" << std::endl;
		bool is_empty = this->send_byte_array_queue.empty();
		this->send_byte_array_queue.push_back(byte_array);
		if(is_empty) { //start new
			this->DoHandleSend();
		}
	}

	auto Close() -> void {
		this->sock.get_io_service().dispatch(
			boost::bind(&Session::DoClose, shared_from_this()));
		//this->DoClose();
	}

private:
	Session(boost::asio::io_service& service, SessionPool::Pointer pool)
		:sock(service), pool(pool){}

	auto DoHandleReceive() -> void {
		std::cout << "do handle receive" << std::endl;
		this->sock.async_read_some(
			boost::asio::buffer(received_byte_array),
			boost::bind(
				&Session::HandleReceive, 
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	auto HandleReceive(const boost::system::error_code& error_code, std::size_t bytes_transferred)
	-> void {
		std::cout << "handle receive" << std::endl;
		if(!error_code){
			std::cout << "received:";
			std::cout.write(&received_byte_array.front(), bytes_transferred);
			std::cout << std::endl;
			DoHandleReceive();
		}
		else if(this->sock.is_open()){ //peer socket closed
			std::cout << "handle receive error. maybe peer socket close." << std::endl;
			this->Close();	
		}
		else{ //self socket closed
			std::cout << "receiving stop" << std::endl;
		}
		
	}

	auto DoHandleSend() -> void {
		boost::asio::async_write(
			this->sock, 
			boost::asio::buffer(
				&send_byte_array_queue.front().front(), 
				send_byte_array_queue.front().size()
			),
			boost::bind(
				&Session::HandleSend,
				this, 
				boost::asio::placeholders::error
			)
		);
	}

	auto HandleSend(const boost::system::error_code& error_code) -> void {
		if(!error_code){
			if(!this->send_byte_array_queue.empty()){
				this->send_byte_array_queue.pop_front();
				this->DoHandleSend();
			}
		}
		else{
			std::cout << "send failed" << std::endl;	
		}
	}

	auto DoClose() -> void {
		std::cout << GetAddressStr(shared_from_this()) << " closed" << std::endl;
		this->sock.close();
		this->pool->Erase(shared_from_this());
	}

	boost::asio::ip::tcp::socket sock;
	SessionPool::Pointer pool;
	std::array<char, 100> received_byte_array;
	std::deque<utl::ByteArray> send_byte_array_queue;

};

}


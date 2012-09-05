#pragma once
//Session:20120816
#include <iostream>
#include <cassert>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "SessionBase.h"

namespace nr{
namespace ntw{

class Session : public SessionBase, public boost::enable_shared_from_this<Session>
{
public:
	using Pointer = boost::shared_ptr<Session>;
	using OnReceiveFunc = boost::function<void (Pointer, const ByteArray&)>;
	using OnCloseFunc = boost::function<void (Pointer)>;

	~Session(){
		this->os << "session deleted." << std::endl;
	}

	static auto Create(
		boost::asio::io_service& service, int buffer_size,
		OnReceiveFunc on_receive_func, OnCloseFunc on_close_func,
		std::ostream& os) -> Session::Pointer {
		return Pointer(new Session(
			service, buffer_size, on_receive_func, on_close_func, os));
	}

	
	auto GetSocketRef() -> boost::asio::ip::tcp::socket& {
		return this->sock;
	}

	auto StartReceive() -> void {
		this->os << "start receive" << std::endl;
		DoOnReceive();
	}

	auto Send(const ByteArray& byte_array) -> void {
		assert(this->sock.is_open());
		this->os << "send" << std::endl;
		bool is_empty = this->send_byte_array_queue.empty();
		this->send_byte_array_queue.push_back(byte_array);
		if(is_empty) { //start new
			this->DoOnSend();
		}
	}

	auto Close() -> void {
		this->sock.get_io_service().post(
			boost::bind(&Session::DoClose, shared_from_this()));
	}


private:
	Session(boost::asio::io_service& service, int buffer_size,
		OnReceiveFunc on_receive_func,
		OnCloseFunc on_close_func,
		std::ostream& os)
			:sock(service), on_receive_func(on_receive_func), on_close_func(on_close_func), 
			part_of_array(buffer_size), received_byte_array(), on_send_strand(service), 
			os(os){}

	auto DoOnReceive() -> void {
		this->os << "do on receive" << std::endl;
		this->sock.async_read_some(
			boost::asio::buffer(part_of_array),
			boost::bind(
				&Session::OnReceive, 
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);

	}

	auto OnReceive(
		const boost::system::error_code& error_code, std::size_t bytes_transferred)
	-> void {
		this->os << "on receive" << std::endl;
		if(!error_code){
			std::copy(part_of_array.begin(), part_of_array.begin()+bytes_transferred, 
				std::back_inserter(this->received_byte_array));
			
			if(bytes_transferred < this->part_of_array.size()){
				this->sock.get_io_service().dispatch(boost::bind(
					this->on_receive_func, this->shared_from_this(), this->received_byte_array));
				this->received_byte_array.resize(0);
			}
			
			DoOnReceive();
		}
		else if(this->sock.is_open()){ //peer socket closed
			this->os << "on receive error. maybe peer socket close." << std::endl;
			this->Close();	
		}
		else{ //self socket closed
			this->os << "receiving stop" << std::endl;
		}
	}

	auto DoOnSend() -> void {
		boost::asio::async_write(
			this->sock, 
			boost::asio::buffer(
				&send_byte_array_queue.front().front(), 
				send_byte_array_queue.front().size()
			),
			this->on_send_strand.wrap(boost::bind(
				&Session::OnSend,
				shared_from_this(), 
				boost::asio::placeholders::error
			))
		);
	}

	auto OnSend(const boost::system::error_code& error_code) -> void {
		if(!error_code){
			if(!this->send_byte_array_queue.empty()){
				this->send_byte_array_queue.pop_front();
				if(!this->send_byte_array_queue.empty()){
					this->DoOnSend();
				}
			}
		}
		else if(error_code == boost::system::errc::connection_reset)
		{
			this->os << "reseted ?" << std::endl;	
		}
		else{
			this->os << "send failed. error code is "<< error_code.message() << std::endl;	
		}
	}

	auto DoClose() -> void {
		this->os << GetRemoteAddressStr(this->shared_from_this()) << ":"
			<< GetRemotePort(this->shared_from_this()) << " closed" << std::endl;
		this->sock.close();
		this->on_close_func(shared_from_this());
	}

	boost::asio::ip::tcp::socket sock;
	OnReceiveFunc on_receive_func;
	OnCloseFunc on_close_func;
	ByteArray part_of_array;
	ByteArray received_byte_array;
	std::deque<ByteArray> send_byte_array_queue;
	boost::asio::strand on_send_strand;
	std::ostream& os;

};


auto CreateTestSession(boost::asio::io_service& service) -> Session::Pointer {
	return Session::Create(service, 128, 
		[](Session::Pointer session, const ByteArray&)
			{ std::cout << "on receive !!!" << std::endl; }, 
		[](Session::Pointer){ std::cout << "on close !!!" << std::endl; }, 
		std::cout);
}

}
}


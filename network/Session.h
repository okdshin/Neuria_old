#pragma once
//Session:20120823
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "../ByteArray.h"
#include "../NodeId.h"

namespace nr{
namespace ntw{

class Session
{
public:
	using Pointer = boost::shared_ptr<Session>;
	using OnReceiveFunc = boost::function<void (Pointer, const ByteArray&)>;
	using OnCloseFunc = boost::function<void (Pointer)>;

	virtual auto GetNodeId() -> NodeId = 0;
	virtual auto StartReceive() -> void = 0;
	virtual auto Send(const ByteArray& byte_array) -> void = 0;
 	virtual auto Close() -> void = 0;
/*	
	auto StartReceive() -> void {
		this->DoStartReceive();
	}

	auto Send(const ByteArray& byte_array) -> void {
		this->DoSend(byte_array);	
	}

	auto Close() -> void {
		this->DoClose();	
	}
	
	virtual ~Session(){}

private:
	virtual auto DoStartReceive() -> void = 0;
	virtual auto DoSend(const ByteArray& byte_array) -> void = 0;
	virtual auto DoClose() -> void = 0;
*/
};


}

}

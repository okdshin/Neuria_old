#pragma once
//SessionBase:20120823
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "../ByteArray.h"

namespace nr{
namespace ntw{

class SessionBase
{
public:
	using Pointer = boost::shared_ptr<SessionBase>;
	virtual ~SessionBase(){}

	virtual auto GetSocketRef() -> boost::asio::ip::tcp::socket& = 0;
	virtual auto Send(const ByteArray& byte_array) -> void = 0;
	virtual auto Close() -> void = 0;

};

auto GetRemoteAddressStr(SessionBase::Pointer session) -> std::string {
	return session->GetSocketRef().remote_endpoint().address().to_string();	
}

auto GetRemotePort(SessionBase::Pointer session) -> int {
	return session->GetSocketRef().remote_endpoint().port();
}

}

}

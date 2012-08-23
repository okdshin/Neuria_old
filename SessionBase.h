#pragma once
//SessionBase:20120823
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "Utility.h"

namespace nr
{

class SessionBase
{
public:
	using Pointer = boost::shared_ptr<SessionBase>;
	virtual ~SessionBase(){}

	virtual auto GetSocketRef() -> boost::asio::ip::tcp::socket& = 0;
	virtual auto Send(const utl::ByteArray& byte_array) -> void = 0;
	virtual auto Close() -> void = 0;
};

auto GetAddressStr(SessionBase::Pointer session) -> std::string {
	return session->GetSocketRef().remote_endpoint().address().to_string();	
}

}


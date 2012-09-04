#pragma once
//ServantInfo:20120903
#include <iostream>

namespace nr
{
class ServantInfo{
public:
    ServantInfo(const std::string& address, int port) 
		: address(address), port(port){}

	auto GetAddress() -> std::string { return address; }
	auto GetPort() -> int { return port; }

private:
	std::string address;
	int port;

};
}


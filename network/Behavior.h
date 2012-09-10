#pragma once
//Behavior:20120908
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Server.h"

namespace nr{
namespace ntw{

class Behavior{
public:
	using Pointer = boost::shared_ptr<Behavior>;
	virtual auto Bind(Server::Pointer server) -> void {
		this->DoBind(server);	
	}

private:
	virtual auto DoBind(Server::Pointer server) -> void = 0;
};

}
}


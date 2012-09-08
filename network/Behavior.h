#pragma once
//Behavior:20120908
#include <iostream>

namespace nr{
namespace ntw{

class Behavior{
public:
	using Pointer = boost::shared_ptr<Behavior>
	virtual auto Bind(Server::Pointer server) -> void {
		this->DoBind(server);	
	}

private:
	virtual auto DoBind(Server::Pointer server) -> void = 0;
};

}
}


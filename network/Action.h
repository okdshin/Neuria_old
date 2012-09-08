#pragma once
//Action:20120909
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Client.h"

namespace nr{
namespace ntw{

class Action{
public:
	using Pointer = boost::shared_ptr<Action>;
	
	auto Bind(Client::Pointer client) -> void {
		this->DoBind(client);	
	}

    virtual ~Action(){}

private:
	virtual auto DoBind(Client::Pointer client) -> void = 0;

};

}
}


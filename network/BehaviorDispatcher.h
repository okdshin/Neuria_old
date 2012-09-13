#pragma once
//BehaviorDispatcher:20120913
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "Server.h"
#include "DispatchCommand.h"

namespace nr{
namespace ntw{

class BehaviorDispatcher : 
		public boost::enable_shared_from_this<BehaviorDispatcher> {
public:
	using Pointer = boost::shared_ptr<BehaviorDispatcher>;
	static auto Create(std::ostream& os) -> Pointer {
		return Pointer(new BehaviorDispatcher(os));	
	}
	
	auto Bind(Server::Pointer server) -> void {
		SetOnReceiveFuncOnly(server, boost::bind(
			&BehaviorDispatcher::Dispatch, this->shared_from_this(), _1, _2));
	}

	auto RegisterFunc(const DispatchCommand::CommandId& command_id, 
			Session::OnReceiveFunc func) -> void{
		this->func_dict[command_id] = func;
	}

private:
    BehaviorDispatcher(std::ostream& os):os(os){}

	auto Dispatch(Session::Pointer session, 
			const ByteArray& byte_array) -> void {
		std::cout << "received dispatch command serialized byte array:" 
			<< utl::ByteArray2String(byte_array) << std::endl;
		auto command = DispatchCommand::Parse(byte_array);
		std::cout << "received dispatch command: " << command << std::endl;
		if(this->func_dict.find(command.GetCommandId()) == this->func_dict.end()){
			this->os << "invalid command id:" << command.GetCommandId() << std::endl;
			return;
		}
		else{
			this->os << "call command(id:" 
				<< command.GetCommandId() << ")" << std::endl;
			this->func_dict[command.GetCommandId()](
				session, command.GetWrappedByteArray());
		}
	}
	
	std::map<DispatchCommand::CommandId, Session::OnReceiveFunc> func_dict;
	std::ostream& os;
};

}
}


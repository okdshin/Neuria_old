#pragma once
//DispatchCommand:20120913
#include <iostream>
#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Session.h"
#include "../ByteArray.h"
#include "../utility/Utility.h"

namespace nr{
namespace ntw{

class DispatchCommand{
public:
	using CommandId = std::string;
    
	DispatchCommand(){}
	DispatchCommand(const CommandId& command_id, const ByteArray& byte_array)
		: command_id(command_id),
		byte_array(byte_array){}

	static auto Parse(const nr::ByteArray& byte_array) -> DispatchCommand {
		std::stringstream ss(nr::utl::ByteArray2String(byte_array));
		boost::archive::text_iarchive ia(ss);
		auto command = DispatchCommand();
		ia >> command;
		return command;
	}
	
	auto Serialize()const -> ByteArray {
		std::stringstream ss;
		boost::archive::text_oarchive oa(ss);
		oa << static_cast<const DispatchCommand&>(*this);
		return nr::utl::String2ByteArray(ss.str());	
	}

	auto GetCommandId()const -> CommandId { return command_id; }
	auto GetWrappedByteArray()const -> ByteArray { return byte_array; }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, unsigned int ver){
		ar & command_id & byte_array;
	}

	CommandId command_id;
	ByteArray byte_array;
};

auto operator<<(std::ostream& os, const DispatchCommand& command) -> std::ostream& {
	os << "command_id:" << command.GetCommandId() << " byte_array:" 
		<< utl::ByteArray2String(command.GetWrappedByteArray()) << std::endl;
	return os;
}

auto SendDispatchCommand(Session::Pointer session, 
		const DispatchCommand::CommandId& command_id, 
		const ByteArray& byte_array) -> void {
	session->Send(DispatchCommand(command_id, byte_array).Serialize());
}

}
}


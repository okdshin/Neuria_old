#pragma once
//CuiShell:20120909
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "../utility/Utility.h"

namespace nr{
namespace utl{

class CuiShell{
public:
	using ArgList = std::vector<std::string>;
	using CalledFunc = std::function<void (const ArgList&)>;

	CuiShell(std::ostream& os):func_dict(), os(os){
		this->Register("help", "shows this help ;)", 
			boost::bind(&CuiShell::Help, this, _1));
	}

	auto Register(const std::string& key, const std::string& explanation, 
			CalledFunc func) -> void {
		this->func_dict[key] = std::make_tuple(explanation, func);
	}

	auto Start() -> void {
		while(true){
			auto command = utl::GetInput<std::string>("command?:");
			auto parsed_list = ArgList();
			boost::split(parsed_list, command, boost::is_any_of(" "));
			try{
				if(this->func_dict.find(parsed_list.at(0)) != this->func_dict.end()){
					this->CallFunc(parsed_list);
				}
				else{
					throw "invalid";
				}
			}
			catch(...){
				this->os << "\"" << command << "\" is invalid command." << std::endl;	
			}
		}
	}

private:
	auto CallFunc(const ArgList& arg_list) -> void {
		std::get<1>(this->func_dict[arg_list.at(0)])(arg_list);
	}

	auto Help(const ArgList& argment_list) -> void {
		for(auto& func_pair : this->func_dict){
			this->os << boost::format("%1%:%2%") 
				% func_pair.first // key
				% std::get<0>(func_pair.second) // explanation 
				<< std::endl;
		}
	}

	std::map<std::string, std::tuple<std::string, CalledFunc>> func_dict;
	std::ostream& os;
};

auto RegisterExitFunc(CuiShell& shell) -> void {
	shell.Register("exit", "exit this app.", [](const CuiShell::ArgList&){exit(0);});
}

}
}

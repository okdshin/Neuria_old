#pragma once
//Utility:20120816
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <boost/random.hpp>
#include "../ByteArray.h"
#include "../NodeId.h"
#include "MatchFuncCaller.h"
#include "LabeledSink.h"
#include "TypeWrapper.h"

namespace nr{
namespace utl{

auto RANDOM = boost::random::mt19937(static_cast<unsigned long>(std::time(0)));
using RandomRange = boost::random::uniform_int_distribution<>;
//RandomRange(1,6) is equalt to dice.

auto Random(int min, int max) -> int {
	return RandomRange(min, max)(RANDOM);
}

auto String2ByteArray(const std::string& str) -> ByteArray {
	return ByteArray(str.begin(), str.end());
}

auto ByteArray2String(const ByteArray& byte_array) -> std::string {
	return std::string(byte_array.begin(), byte_array.end());
}

auto CreateSocketNodeId(const std::string& hostname, int port) -> NodeId {
	return hostname + "/" + boost::lexical_cast<std::string>(port);	
}

class RandomElementSelector{
public:
	RandomElementSelector()
		: gen(boost::mt19937(static_cast<unsigned long>(std::time(0)))){}

	template<class Container>
	auto operator()(Container& container) -> typename Container::iterator::value_type {
		auto dst = boost::uniform_smallint<>(
			0, container.end()-container.begin()-1);//small_intで十分
		auto rand = boost::variate_generator<
			boost::mt19937&, boost::uniform_smallint<>>(this->gen, dst);
		return *(container.begin() + rand());
	}

private:
	boost::mt19937 gen;
};

template<class ValueType>
auto GetInput(const std::string& prompt) -> ValueType
{
	std::cout << prompt << std::flush;
	std::string input;
	std::getline(std::cin, input);
	try
	{
		return boost::lexical_cast<ValueType>(input);
	}
	catch(boost::bad_lexical_cast& e)
	{
		std::cout << "error. retry." << "\n";
		return GetInput<ValueType>(prompt);	
	}
}

auto StrJoin(const std::vector<std::string>& str_vect, std::string delim) -> std::string
{
	auto integrated = str_vect.front();
	for(unsigned int i = 1; i < str_vect.size(); i++)
	{
		integrated = integrated + delim + str_vect.at(i);
	}
	return integrated;
}

}
}

#pragma once
//Utility:20120816
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

namespace utl
{
using ByteArray = std::vector<char>;

auto String2ByteArray(const std::string& str) -> ByteArray {
	return ByteArray(str.begin(), str.end());
}

auto ByteArray2String(const ByteArray& byte_array) -> std::string {
	return std::string(byte_array.begin(), byte_array.end());
}

template<class ValueType>
auto GetInput(const std::string& prompt) -> ValueType
{
	//DEBUG_STREAM(50) << "hello" << 55 << std::endl;
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


class DebugOutStream
{
public:
	DebugOutStream();
	void SetIsEnable(bool is_enable)
	{
		this->is_enable = is_enable;	
	}

	bool IsEnable()const
	{
		return this->is_enable;	
	}

private:
	std::ostream& DebugOut;
	bool is_enable;
};
}


#pragma once
//Utility:20120816
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

namespace utl
{
using ByteArray = std::vector<char>;

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
	std::string integrated = str_vect.front();
	for(unsigned int i = 1; i < str_vect.size(); i++)
	{
		integrated = integrated + delim + str_vect.at(i);
	}
	return integrated;
}
}


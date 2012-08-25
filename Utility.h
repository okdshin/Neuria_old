#pragma once
//Utility:20120816
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

//#define DEBUG_MODE
//#define DEBUG_LEVEL 3
#ifdef DEBUG_MODE
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif
#define DEBUG_PRINT(message, level) \
{ \
	if(DEBUG_LEVEL < level){ \
		std::cout << message << ":" << __FILE__ << ":" << __LINE__ << std::endl; \
	}\
}
#else
#define DEBUG_PRINT(message)
#endif

namespace utl
{
using ByteArray = std::vector<char>;

template<class ValueType>
auto GetInput(const std::string& prompt) -> ValueType
{
	//DEBUG_PRINT("debug message", 4);
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


#pragma once
//Utility:20120816
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <boost/random.hpp>

namespace utl
{
using ByteArray = std::vector<char>;
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

class RandomElementSelector{
public:
	RandomElementSelector()
		: gen(boost::mt19937(static_cast<unsigned long>(std::time(0)))){}

	template<class Container>
	auto operator()(
			Container& container) -> typename Container::iterator::value_type {
		auto dst = boost::uniform_smallint<>(
			0, container.end()-container.begin()-1);//small_intで十分
		auto rand = boost::variate_generator<
			boost::mt19937&, boost::uniform_smallint<>>(this->gen, dst);
		return *(container.begin() + rand());
	}

private:
	boost::mt19937 gen;
};

auto CalcSimilarity(const std::string& left, const std::string& right) -> double {
	double shorter_length = left.length() < right.length() ? left.length() : right.length();
	double longer_length = left.length() > right.length() ? left.length() : right.length();
	auto str = left.length() < right.length() ? left : right;
	auto similarity_unit = shorter_length*shorter_length/longer_length;
	return (std::string::npos != left.find(right)) 
		|| (std::string::npos != right.find(left)) 
			? shorter_length*shorter_length/longer_length : -similarity_unit;
	
}

auto CalcSimilarity(
		const std::vector<std::string>& search_keyward_list, const std::string& target_keyward) -> double {
	double similarity = 0.0;
	for(const auto& search_keyward : search_keyward_list){
		similarity += utl::CalcSimilarity(search_keyward, target_keyward);
	}
	return similarity
		/ std::max_element(search_keyward_list.begin(), search_keyward_list.end(), 
			[](const std::string& left, const std::string& right)
				{ return left.size() < right.size(); }
			)->size();
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

}


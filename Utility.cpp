#ifdef UTILITY_UNIT_TEST
#include "Utility.h"
#include <iostream>

using namespace utl;
/*
void TestRandomElementSelector()
{
	auto selector = RandomElementSelector();
	for(unsigned int i = 0; i < 10; i++){
		std::cout << selector(std::vector<int>{10, 20, 30}) << std::endl;
	}
}
*/
void TestGetInput()
{
	std::cout << GetInput<int>("input number:") << std::endl;
	std::cout << GetInput<std::string>("input string:") << std::endl;
}

void TestCalcSimilarity()
{
	std::cout << CalcSimilarity("abcdefg", "abcdefg") << std::endl;	
	std::cout << CalcSimilarity("abcd", "abcdef") << std::endl;	
	std::cout << CalcSimilarity("abcdef", "abcd") << std::endl;	
	std::cout << CalcSimilarity("abcd", "abcdefg") << std::endl;	
	std::cout << CalcSimilarity("abc", "abcd") << std::endl;	
	std::cout << CalcSimilarity("xyz", "abcd") << std::endl;	
}

int main(int argc, char* argv[])
{
	TestCalcSimilarity();
    return 0;
}

#endif

#ifdef KEYHASHDATABASE_UNIT_TEST
#include "KeyHashDataBase.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	auto database = KeyHashDataBase(0.3, std::cout);
	database.Add("abc", "hashdata");
	database.Add("xyzw", "hashdataaa");
	database.Add("abcde", "hashdataaaa");
	database.Add("abcdefghi", "hashdata");
	database.Erase("hashdata");
	database.Clear();
	auto key_hash_list1 = database.SearchKeyHash(KeywardList{"xyz", "d", "e"});
	auto key_hash_list2 = database.SearchKeyHash(KeywardList{"abcde"});
	std::cout << key_hash_list1 << std::endl;
	std::cout << key_hash_list2 << std::endl;
    return 0;
}

#endif

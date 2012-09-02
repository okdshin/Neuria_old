#ifdef KEYHASHDATABASE_UNIT_TEST
#include "KeyHashDataBase.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	auto database = KeyHashDataBase(0.3, std::cout);
	database.Add("abc", utl::String2ByteArray("hashdata"));
	database.Add("xyzw", utl::String2ByteArray("hashdataaa"));
	database.Add("abcde", utl::String2ByteArray("hashdataaaa"));
	database.Add("abcdefghi", utl::String2ByteArray("hashdata"));
	database.EraseSameAsSrcData(utl::String2ByteArray("hashdata"));
	//database.Clear();
	auto key_hash_list1 = database.Search(KeywardList{"xyz", "d", "e"});
	auto key_hash_list2 = database.Search(KeywardList{"abcde"});
	std::cout << key_hash_list1 << std::endl;
	std::cout << key_hash_list2 << std::endl;
    return 0;
}

#endif

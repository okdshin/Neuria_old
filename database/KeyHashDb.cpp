#ifdef KEYHASHDB_UNIT_TEST
#include "KeyHashDb.h"
#include <iostream>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	auto database = KeyHashDb(0.3, std::cout);
	database.Add(KeyHash::Keyward("abc"), utl::String2ByteArray("hashdata"));
	database.Add(KeyHash::Keyward("xyzw"), utl::String2ByteArray("hashdataaa"));
	database.Add(KeyHash::Keyward("abcde"), utl::String2ByteArray("hashdataaaa"));
	database.Add(KeyHash::Keyward("abcdefghi"), utl::String2ByteArray("hashdata"));
	//database.EraseSameAsSrcData(utl::String2ByteArray("hashdata"));
	//database.Clear();
	
	auto key_hash_list1 = database.Search(StrList2KeywardList({"xyz", "d", "e"}));
	auto key_hash_list2 = database.Search(StrList2KeywardList({"abcde"}));
	std::cout << key_hash_list1 << std::endl;
	std::cout << key_hash_list2 << std::endl;
    
	return 0;
}

#endif

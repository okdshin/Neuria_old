#ifdef FILEKEYHASH_UNIT_TEST
#include "FileKeyHash.h"
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	auto src_key_hash = CreateTestFileKeyHash();
	std::cout << "src:" << src_key_hash << std::endl;
	std::cout << src_key_hash.GetBirthTime() << std::endl; 

	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << (const FileKeyHash&)src_key_hash;

	std::cout << "text serialized:" << ss.str() << std::endl;

	auto dst_key_hash = FileKeyHash();
	boost::archive::text_iarchive ia(ss);
	ia >> dst_key_hash;
	std::cout << "dst:" << dst_key_hash << std::endl;
	std::cout << dst_key_hash.GetBirthTime() << std::endl; 

    return 0;
}

#endif

#ifdef KEYHASH_UNIT_TEST
#include "KeyHash.h"
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	auto src_key_hash = CreateTestKeyHash();
	std::cout << "src:" << src_key_hash << std::endl;

	std::stringstream ss;
	boost::archive::text_oarchive oa(ss);
	oa << (const KeyHash&)src_key_hash;

	std::cout << "text serialized:" << ss.str() << std::endl;

	auto dst_key_hash = KeyHash();
	boost::archive::text_iarchive ia(ss);
	ia >> dst_key_hash;
	std::cout << "dst:" << dst_key_hash << std::endl;

    return 0;
}

#endif

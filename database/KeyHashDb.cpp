#ifdef KEYHASHDB_UNIT_TEST
#include "KeyHashDb.h"
#include <iostream>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	auto database = KeyHashDb(0.3, std::cout);
	return 0;
}

#endif

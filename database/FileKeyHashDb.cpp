#ifdef FILEKEYHASHDB_UNIT_TEST
#include "FileKeyHashDb.h"
#include <iostream>
#include "../utility/Utility.h"

using namespace nr;
using namespace nr::db;

int main(int argc, char* argv[])
{
	auto database = FileKeyHashDb::Create(0.3, 128, std::cout);
	return 0;
}

#endif

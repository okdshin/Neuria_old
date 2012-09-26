#ifdef SESSIONPOOL_UNIT_TEST
#include "SessionPool.h"
#include <iostream>

using namespace nr;
using namespace nr::ntw;

int main(int argc, char* argv[])
{
	auto pool = SessionPool::Create();
	
    return 0;
}

#endif

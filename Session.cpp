#ifdef SESSION_UNIT_TEST
#include "Session.h"
#include "SessionPool.h"
#include <iostream>

using namespace nr;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	auto pool = SessionPool::Create();
	auto session = Session::Create(service, pool);
	service.run();
	session->StartReceive();	
	
	while(true)
	{
	}
    return 0;
}

#endif

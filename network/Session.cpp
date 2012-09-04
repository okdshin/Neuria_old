#ifdef SESSION_UNIT_TEST
#include "Session.h"
#include <iostream>

//using namespace nr;
using namespace nr::ntw;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	auto session = CreateTestSession(service); 
	service.run();
	session->StartReceive();	
	
    return 0;
}

#endif

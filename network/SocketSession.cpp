#ifdef SOCKETSESSION_UNIT_TEST
#include "SocketSession.h"
#include <iostream>

//using namespace nr;
using namespace nr::ntw;

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	auto session = CreateTestSocketSession(service); 
	service.run();
	session->StartReceive();	
	
    return 0;
}

#endif

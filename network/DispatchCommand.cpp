#ifdef DISPATCHCOMMAND_UNIT_TEST
#include "DispatchCommand.h"
#include <iostream>

using namespace nr;
using namespace nr::ntw;

int main(int argc, char* argv[])
{
	auto command = DispatchCommand("command_id", 
		utl::String2ByteArray("byte_arraydfghjkhgfdgh"));
	//std::cout << utl::ByteArray2String(command.Serialize()) << std::endl;
	std::cout << "parsed:" << DispatchCommand::Parse(command.Serialize()) << std::endl;

    return 0;
}

#endif

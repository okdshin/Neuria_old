#ifdef SHELL_UNIT_TEST
#include "Shell.h"
#include <iostream>

using namespace nr;
using namespace nr::utl;

int main(int argc, char* argv[])
{
	auto shell = Shell(std::cout);
	RegisterExitFunc(shell);
	shell.Register("broadcast", "broadcast text to peers.", 
		[](const Shell::ArgumentList& argument_list){
			std::cout << "call broadcast." << std::endl;
			for(const auto& argument : argument_list){
				std::cout << argument << std::endl;	
			}
		});
	shell.Start();

    return 0;
}

#endif

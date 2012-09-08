#ifdef CUISHELL_UNIT_TEST
#include "CuiShell.h"
#include <iostream>

using namespace nr;
using namespace nr::utl;

int main(int argc, char* argv[])
{
	auto shell = CuiShell(std::cout);
	RegisterExitFunc(shell);
	shell.Register("broadcast", "broadcast text to peers.", 
		[](const CuiShell::ArgList& arg_list){
			std::cout << "call broadcast." << std::endl;
			for(const auto& arg : arg_list){
				std::cout << arg << std::endl;	
			}
		});
	shell.Start();

    return 0;
}

#endif

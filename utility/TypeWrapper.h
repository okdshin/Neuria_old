#pragma once
//TypeWrapper:20120920
#include <iostream>

namespace nr{
namespace utl{ 

template<class Type, class Id>
class TypeWrapper
{
public:
	TypeWrapper(const Type& val) : val(val) {}
	auto operator()() -> Type& { return val; }

private:
	Type val;
};

}
}


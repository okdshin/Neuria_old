#pragma once
//Hash:20120823
#include <iostream>
#include <iomanip>
#include <array>
#include <sstream>
#include <openssl/sha.h>
#include "../ByteArray.h"

namespace nr{
namespace db{

auto CalcHashStr(const ByteArray& data) -> std::string
{
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &data.front(), data.size());

	auto hash = std::array<unsigned char, SHA256_DIGEST_LENGTH>();
	SHA256_Final(&hash.front(), &sha256);
	
	std::stringstream ss;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}

}
}


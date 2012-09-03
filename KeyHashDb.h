#pragma once
//KEYHASHDB:20120831
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include "Utility.h"
#include "KeyHash.h"
#include "Hash.h"

namespace nr
{

auto CalcId(const utl::ByteArray& src_data) -> KeyHash::Id {
	return KeyHash::Id(GetHashStr(src_data));	
}

using KeywardList = std::vector<KeyHash::Keyward>;

auto StrList2KeywardList(
		const std::vector<std::string>& keyward_str_list) -> KeywardList {
	auto keyward_list = KeywardList();
	std::transform(
		keyward_str_list.begin(), keyward_str_list.end(), 
		std::back_inserter(keyward_list), 
		[](const std::string& keyward_str){
			return KeyHash::Keyward(keyward_str);
		});
	return keyward_list;
}

auto CalcSimilarity(
		const KeywardList& keyward_list, const KeyHash::Keyward& keyward) -> double{
	auto keyward_str_vec = std::vector<std::string>();
	std::transform(
		keyward_list.begin(), keyward_list.end(), 
		std::back_inserter(keyward_str_vec), 
		[](const KeyHash::Keyward& keyward){
			return keyward.Get();
		});
	return utl::CalcSimilarity(keyward_str_vec, keyward.Get());
}

class KeyHashDb{
public:
	using KeyHashList = std::vector<KeyHash>;

	KeyHashDb(double threshold, std::ostream& os)
		:hash_list(), threshold(threshold), os(os){}

	auto Add(const KeyHash::Keyward& keyward, const utl::ByteArray& src_data) -> void {
		this->hash_list.push_back(KeyHash(
			KeyHash::Keyward(keyward), KeyHash::Id(CalcId(src_data))));
	}

	auto EraseSameAsKeyHashData(const KeyHash::Id& data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&data](const KeyHash& key_hash){return key_hash.GetId() == data;}), 
			this->hash_list.end());	
	}

	auto EraseSameAsSrcData(const utl::ByteArray& src_data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&src_data](const KeyHash& key_hash)
					{return key_hash.GetId() == CalcId(src_data);}), 
			this->hash_list.end());	
	}

	auto Clear() -> void {
		this->hash_list.clear();	
	}

	auto Search(const KeywardList& search_keyward_list) -> KeyHashList {
		for(const auto& key_hash : this->hash_list){
			this->os << key_hash.GetKeywardStr() << "|";
			for(const auto& keyward : search_keyward_list){
				this->os << keyward << " ";	
			}
			this->os << "/similarity:" 
				<< CalcSimilarity(
					search_keyward_list, key_hash.GetKeyward()) << std::endl;
		}
		auto end = std::partition(this->hash_list.begin(), this->hash_list.end(), 
			[&](const KeyHash& key_hash){
				return CalcSimilarity(
					search_keyward_list, key_hash.GetKeyward()) > this->threshold;	
			});
		return KeyHashList(this->hash_list.begin(), end);
	}

private:
	KeyHashList hash_list;
	double threshold;
	std::ostream& os;
};

std::ostream& operator<<(std::ostream& os, const KeyHashDb::KeyHashList& key_hash_list){
	for(const auto& key_hash : key_hash_list)
	{
		os << key_hash.GetKeyward() << ":" << key_hash.GetId() << " ";
	}

	return os;
}
}


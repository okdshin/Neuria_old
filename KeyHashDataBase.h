#pragma once
//KeyHashDataBase:20120831
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include "Utility.h"
#include "Hash.h"

namespace nr
{
using KeyHashData = std::string;
using Keyward = std::string;
using KeywardList = std::vector<Keyward>;
using KeyHash = std::tuple<Keyward, KeyHashData>;

auto GetData(const KeyHash& key_hash) -> KeyHashData {
	return std::get<1>(key_hash);	
}

auto GetKeyward(const KeyHash& key_hash) -> Keyward {
	return std::get<0>(key_hash);
}

using KeyHashList = std::vector<KeyHash>;
std::ostream& operator<<(std::ostream& os, const KeyHashList& key_hash_list){
	for(const auto& key_hash : key_hash_list)
	{
		os << GetKeyward(key_hash) << ":" << GetData(key_hash) << " ";
	}

	return os;
}

auto CalcSimilarity(
		const KeywardList& search_keyward_list, const Keyward& target_keyward) -> double {
	double similarity = 0.0;
	for(const auto& search_keyward : search_keyward_list){
		similarity += utl::CalcSimilarity(search_keyward, target_keyward);
	}
	return similarity
		/ std::max_element(search_keyward_list.begin(), search_keyward_list.end(), 
			[](const Keyward& left, const Keyward& right)
				{return left.size() < right.size();}
			)->size();
}

auto CalcKeyHashData(const utl::ByteArray& src_data) -> KeyHashData {
	return GetHashStr(src_data);	
}

class KeyHashDataBase{
public:
    KeyHashDataBase(double threshold, std::ostream& os)
		:hash_list(), threshold(threshold), os(os){}

	auto Add(const Keyward& keyward, const utl::ByteArray& src_data) -> void {
		this->hash_list.push_back(std::make_tuple(keyward, CalcKeyHashData(src_data)));
	}

	auto EraseSameAsKeyHashData(const KeyHashData& data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&data](const KeyHash& hash){return GetData(hash) == data;}), 
			this->hash_list.end());	
	}

	auto EraseSameAsSrcData(const utl::ByteArray& src_data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&src_data](const KeyHash& hash)
					{return GetData(hash) == CalcKeyHashData(src_data);}), 
			this->hash_list.end());	
	}

	auto Clear() -> void {
		this->hash_list.clear();	
	}

	auto Search(const KeywardList& search_keyward_list) -> KeyHashList {
		for(const auto& key_hash : this->hash_list){
			this->os <<  GetKeyward(key_hash) << "|";
			for(const auto& keyward : search_keyward_list){
				this->os << keyward << " ";	
			}
			this->os << "/similarity:" 
				<< CalcSimilarity(search_keyward_list, GetKeyward(key_hash)) << std::endl;
		}
		auto end = std::partition(this->hash_list.begin(), this->hash_list.end(), 
			[&](const KeyHash& key_hash){
				return CalcSimilarity(
					search_keyward_list, GetKeyward(key_hash)) > this->threshold;	
			});
		return KeyHashList(this->hash_list.begin(), end);
	}

private:
	KeyHashList hash_list;
	double threshold;
	std::ostream& os;
};

}


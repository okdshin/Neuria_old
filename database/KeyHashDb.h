#pragma once
//KEYHASHDB:20120831
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include "KeyHash.h"
#include "Hash.h"

namespace nr{
namespace db{

auto CalcId(const ByteArray& src_data) -> KeyHash::KeyHashId {
	return KeyHash::KeyHashId(GetHashStr(src_data));	
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

auto CalcSimilarity(const std::string& left, const std::string& right) -> double {
	double shorter_length = left.length() < right.length() ? left.length() : right.length();
	double longer_length = left.length() > right.length() ? left.length() : right.length();
	auto str = left.length() < right.length() ? left : right;
	auto similarity_unit = shorter_length*shorter_length/longer_length;
	return (std::string::npos != left.find(right)) 
		|| (std::string::npos != right.find(left)) 
			? shorter_length*shorter_length/longer_length : -similarity_unit;
	
}

auto CalcSimilarity(
		const std::vector<std::string>& search_keyward_list, const std::string& target_keyward) -> double {
	double similarity = 0.0;
	for(const auto& search_keyward : search_keyward_list){
		similarity += CalcSimilarity(search_keyward, target_keyward);
	}
	return similarity
		/ std::max_element(search_keyward_list.begin(), search_keyward_list.end(), 
			[](const std::string& left, const std::string& right)
				{ return left.size() < right.size(); }
			)->size();
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
	return CalcSimilarity(keyward_str_vec, keyward.Get());
}

class KeyHashDb{
public:
	using KeyHashList = std::vector<KeyHash>;

	KeyHashDb(double threshold, std::ostream& os)
		:hash_list(), threshold(threshold), os(os){}

	auto Add(const KeyHash::Keyward& keyward, const ByteArray& src_data) -> void {
		this->hash_list.push_back(KeyHash(
			KeyHash::Keyward(keyward), KeyHash::KeyHashId(CalcId(src_data))));
	}

	auto EraseSameAsKeyHashData(const KeyHash::KeyHashId& data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&data](const KeyHash& key_hash){return key_hash.GetId() == data;}), 
			this->hash_list.end());	
	}

	auto EraseSameAsSrcData(const ByteArray& src_data) -> void {
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
}

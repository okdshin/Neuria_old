#pragma once
//FILEKEYHASHDB:20120831
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <boost/format.hpp>
#include "FileKeyHash.h"
#include "Hash.h"

namespace nr{
namespace db{

auto CalcId(const ByteArray& src_data) -> FileKeyHash::HashId {
	return FileKeyHash::HashId(CalcHashStr(src_data));	
}

using KeywardList = std::vector<FileKeyHash::Keyward>;

auto StrList2KeywardList(
		const std::vector<std::string>& keyward_str_list) -> KeywardList {
	auto keyward_list = KeywardList();
	std::transform(
		keyward_str_list.begin(), keyward_str_list.end(), 
		std::back_inserter(keyward_list), 
		[](const std::string& keyward_str){
			return FileKeyHash::Keyward(keyward_str);
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

auto CalcSimilarity(const KeywardList& keyward_list, 
		const FileKeyHash::Keyward& keyward) -> double{
	auto keyward_str_vec = std::vector<std::string>();
	std::transform(
		keyward_list.begin(), keyward_list.end(), 
		std::back_inserter(keyward_str_vec), 
		[](const FileKeyHash::Keyward& keyward){
			return keyward.Str();
		});
	return CalcSimilarity(keyward_str_vec, keyward.Str());
}

class FileKeyHashDb{
public:
	using FileKeyHashList = std::vector<FileKeyHash>;
	using KeywardList = std::vector<FileKeyHash::Keyward>;
	using Pointer = boost::shared_ptr<FileKeyHashDb>;
	static auto Create(double threshold, int buffer_size, 
			std::ostream& os) -> Pointer {
		return Pointer(new FileKeyHashDb(threshold, buffer_size, os));	
	}

	auto Add(const FileKeyHash& key_hash) -> void {
		auto is_equal_to_key_hash = 
			[&key_hash](const FileKeyHash& e) -> bool {
				return key_hash.GetHashId() == e.GetHashId(); };

		// when same id has already existed.
		if(std::find_if(this->hash_list.begin(), this->hash_list.end(), 
				is_equal_to_key_hash) != this->hash_list.end()){
			std::replace_if(this->hash_list.begin(), this->hash_list.end(), 
				is_equal_to_key_hash, key_hash);
		}
		else{
			this->hash_list.push_back(key_hash);
		}
	}

	auto Add(const FileKeyHashList& key_hash_list) -> void {
		/*
		std::copy(key_hash_list.begin(), key_hash_list.end(), 
			std::back_inserter(this->hash_list));
		*/
		for(auto& key_hash : key_hash_list){
			this->Add(key_hash);
		}
	}
	
	auto Add(const FileKeyHash::Keyward& keyward, 
			const boost::filesystem::path& file_path, 
			const nr::NodeId& node_id) -> void {
		this->Add(FileKeyHash(FileKeyHash::HashId(CalcId(
				SerializeFile(file_path, this->buffer_size))), 
			keyward, node_id, file_path));
	}


	auto Search(const KeywardList& search_keyward_list) -> FileKeyHashList {
		for(const auto& key_hash : this->hash_list){
			this->os << key_hash.GetKeyward().Str() << "|";
			for(const auto& keyward : search_keyward_list){
				this->os << keyward << " ";	
			}
			this->os << "/similarity:" 
				<< CalcSimilarity(
					search_keyward_list, key_hash.GetKeyward()) << std::endl;
		}
		auto end = std::partition(this->hash_list.begin(), this->hash_list.end(), 
			[&](const FileKeyHash& key_hash){
				return CalcSimilarity(
					search_keyward_list, key_hash.GetKeyward()) > this->threshold;	
			});
		return FileKeyHashList(this->hash_list.begin(), end);
	}

	auto Get(const FileKeyHash::HashId& hash_id) -> FileKeyHash {
		auto found = std::find_if(this->hash_list.begin(), this->hash_list.end(), 
			[&hash_id](const FileKeyHash& key_hash){
				return key_hash.GetHashId() == hash_id; });	
		if(found == this->hash_list.end()){
			assert(!"not found");	
		}
		return *found;
	}

	auto Erase(const FileKeyHash::HashId& hash_id) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&hash_id](const FileKeyHash& key_hash){
					return key_hash.GetHashId() == hash_id; }), 
			this->hash_list.end());	
	}

	auto Erase(const ByteArray& src_data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&src_data](const FileKeyHash& key_hash)
					{ return key_hash.GetHashId() == CalcId(src_data); }), 
			this->hash_list.end());	
	}

	auto Clear() -> void {
		this->hash_list.clear();	
	}

private:
	friend auto operator<<(std::ostream& os, 
		const FileKeyHashDb::Pointer file_key_hash_db) -> std::ostream&;

	FileKeyHashDb(double threshold, int buffer_size, std::ostream& os)
		:hash_list(), threshold(threshold), buffer_size(buffer_size), os(os){}

	FileKeyHashList hash_list;
	double threshold;
	int buffer_size;
	std::ostream& os;
};

auto operator<<(std::ostream& os, 
		const FileKeyHashDb::Pointer file_key_hash_db) -> std::ostream& {
	for(const auto& key_hash : file_key_hash_db->hash_list){	
		os << key_hash << std::endl;
	}

	return os;
}

}
}

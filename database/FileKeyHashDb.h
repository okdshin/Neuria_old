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

auto CalcId(const ByteArray& src_data) -> HashId {
	return HashId(CalcHashStr(src_data));	
}

class KeywardListType{};
using KeywardList = 
	utl::TypeWrapper<std::vector<Keyward::WrappedType>, KeywardListType>;

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
		const Keyward& keyward) -> double{
	return CalcSimilarity(keyward_list(), keyward());
}

using FileKeyHashList = std::vector<FileKeyHash>;

class FileKeyHashDb{
public:
	using Pointer = boost::shared_ptr<FileKeyHashDb>;
	static auto Create(double threshold, int buffer_size, 
			std::ostream& os) -> Pointer {
		return Pointer(new FileKeyHashDb(threshold, buffer_size, os));	
	}

	auto Add(const FileKeyHash& key_hash) -> void {
		auto is_equal_to_key_hash = 
			[&key_hash](const FileKeyHash& e) -> bool {
				return key_hash.GetHashId()() == e.GetHashId()(); };

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
		for(auto& key_hash : key_hash_list){
			this->Add(key_hash);
		}
	}
	
	auto Add(const Keyward& keyward, 
			const boost::filesystem::path& file_path, 
			const nr::NodeId& node_id) -> void {
		this->Add(FileKeyHash(HashId(CalcId(
				SerializeFile(file_path, this->buffer_size))), 
			keyward, node_id, file_path));
	}

	auto Search(const KeywardList& search_keyward_list) -> FileKeyHashList {
		for(const auto& key_hash : this->hash_list){
			this->os << key_hash.GetKeyward() << "|";
			for(const auto& keyward : search_keyward_list()){
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

	auto Get(const HashId& hash_id) -> FileKeyHash {
		auto found = std::find_if(this->hash_list.begin(), this->hash_list.end(), 
			[&hash_id](const FileKeyHash& key_hash){
				return key_hash.GetHashId()() == hash_id(); });	
		if(found == this->hash_list.end()){
			assert(!"not found");	
		}
		return *found;
	}

	auto GetNewer(unsigned int max_count) -> FileKeyHashList {
		if(max_count > this->hash_list.size()){
			return this->hash_list;	
		}

		std::nth_element(this->hash_list.begin(), 
			this->hash_list.begin()+max_count, 
			this->hash_list.end(), 
			[](const FileKeyHash& left, const FileKeyHash& right){
				return left.GetBirthTime() < right.GetBirthTime();	
			});
		auto newer_list = FileKeyHashList();
		std::copy(this->hash_list.begin(), 
			this->hash_list.begin()+max_count, std::back_inserter(newer_list));
		return newer_list;
	}

	auto Erase(const HashId& hash_id) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&hash_id](const FileKeyHash& key_hash){
					return key_hash.GetHashId()() == hash_id(); }), 
			this->hash_list.end());	
	}

	auto Erase(const ByteArray& src_data) -> void {
		this->hash_list.erase(
			std::remove_if(this->hash_list.begin(), this->hash_list.end(), 
				[&src_data](const FileKeyHash& key_hash)
					{ return key_hash.GetHashId()() == CalcId(src_data)(); }), 
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

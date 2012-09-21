#pragma once
//FileKeyHash:20120903
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../NodeId.h"
#include "../ByteArray.h"


namespace nr{
namespace db{

auto SerializeFile(const boost::filesystem::path& file_path, 
		int buffer_size) -> ByteArray {
	boost::filesystem::ifstream ifs(file_path, std::ios::binary);
	auto whole_byte_array = nr::ByteArray();
	auto part_byte_array = nr::ByteArray(buffer_size);
	int count = 0;
	while(true){
		//std::cout << "readsome" << std::endl;
		++count;
		const auto read_size = ifs.readsome(&part_byte_array.front(), buffer_size);
		std::copy(part_byte_array.begin(), part_byte_array.begin()+read_size, 
			std::back_inserter(whole_byte_array));
		if(read_size < buffer_size){ break; }
	}
	ifs.close();
	std::cout << count*buffer_size << " bytes." << std::endl;
	return whole_byte_array;
}

class FileKeyHash{
public:
	class HashId{
		public:
			HashId(){}
			explicit HashId(const std::string& val):val(val){}
			auto Str()const -> std::string { return this->val; }

		private:
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, unsigned int ver){
				ar & val;
			}
			std::string val;
	};

	class Keyward{
		public:
			Keyward(){}
			explicit Keyward(const std::string& val):val(val){}
			auto Str()const -> std::string { return this->val; }

		private:
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, unsigned int ver){
				ar & val;
			}
			std::string val;
	};
   
   	FileKeyHash(){}
	FileKeyHash(const HashId& hash_id, const Keyward& keyward, 
			const NodeId& owner_id, const boost::filesystem::path& file_path) 
		:hash_id(hash_id), keyward(keyward), owner_id_str(owner_id), 
		file_path_str(file_path.string()){
		this->birth_universal_time_str 
			= boost::posix_time::to_simple_string(
				boost::posix_time::second_clock::universal_time());
	}
	
	auto GetHashId() const -> HashId { return hash_id; }
	auto GetKeyward() const -> Keyward { return keyward; }
	auto GetOwnerId() const -> nr::NodeId { return nr::NodeId(this->owner_id_str); }
	auto GetFilePath() const -> boost::filesystem::path { 
		return boost::filesystem::path(this->file_path_str); }
	auto GetBirthTime() const -> boost::posix_time::ptime { 
		return boost::posix_time::time_from_string(this->birth_universal_time_str); }

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, unsigned int ver){
		ar & hash_id & keyward & owner_id_str 
			& file_path_str & birth_universal_time_str;
	}
	
	HashId hash_id;
	Keyward keyward;
	std::string owner_id_str;
	std::string file_path_str;
	std::string birth_universal_time_str;
};

auto CreateTestFileKeyHash() -> FileKeyHash {
	return FileKeyHash(FileKeyHash::HashId("hash_id"), 
		FileKeyHash::Keyward("keyward"), NodeId("owner_node_id"), "./test_file.dat");
}

auto operator <<(std::ostream& os, const FileKeyHash::HashId& id) -> std::ostream& {
	os << id.Str();
	return os;
}

auto operator <<(std::ostream& os, 
		const FileKeyHash::Keyward& keyward) -> std::ostream& {
	os << keyward.Str();
	return os;
}

auto operator << (std::ostream& os, const FileKeyHash& key_hash) -> std::ostream& {
	os << boost::format(
		"Keyward:%1%, HashId:%2%, OwnerId:%3%, FilePath:%4%, BirthUniversalTime:%5%") 
		% key_hash.GetKeyward() 
		% key_hash.GetHashId() 
		% key_hash.GetOwnerId() 
		% key_hash.GetFilePath() 
		% boost::posix_time::to_simple_string(key_hash.GetBirthTime()) << std::endl;
	return os;
}

auto operator==(const FileKeyHash::HashId& left, 
		const FileKeyHash::HashId& right) -> bool {
	return left.Str() == right.Str();
}

}
}

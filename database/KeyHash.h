#pragma once
//KeyHash:20120903
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>
#include "../NodeId.h"


namespace nr{
namespace db{

class KeyHash{
public:
	class HashId{
		public:
			HashId(){}
			explicit HashId(const std::string& val):val(val){}
			auto Get()const -> std::string { return this->val; }

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
			auto Get()const -> std::string { return this->val; }

		private:
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive& ar, unsigned int ver){
				ar & val;
			}
			std::string val;
	};
   
   	KeyHash(){}
	KeyHash(const HashId& hash_id, const Keyward& keyward, const NodeId& owner_id) 
		:hash_id(hash_id), keyward(keyward), owner_id(owner_id){}
	
	auto GetHashId()const -> HashId { return hash_id; }
	auto GetKeyward()const -> Keyward { return keyward; }
	auto GetOwnerId()const -> nr::NodeId { return owner_id; }
	
	auto GetHashIdStr()const -> std::string { return hash_id.Get(); }
	auto GetKeywardStr()const -> std::string { return keyward.Get(); }


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, unsigned int ver){
		ar & hash_id & keyward & owner_id;
	}
	
	HashId hash_id;
	Keyward keyward;
	NodeId owner_id;

};

auto CreateTestKeyHash() -> KeyHash {
	return KeyHash(KeyHash::HashId("hash_id"), 
		KeyHash::Keyward("keyward"), NodeId("owner_node_id"));	
}

auto operator <<(std::ostream& os, const KeyHash::HashId& id) -> std::ostream& {
	os << id.Get();
	return os;
}

auto operator <<(std::ostream& os, const KeyHash::Keyward& keyward) -> std::ostream& {
	os << keyward.Get();
	return os;
}

auto operator << (std::ostream& os, const KeyHash& key_hash) -> std::ostream& {
	os << key_hash.GetKeyward() << ":" << key_hash.GetHashId() << ":" << key_hash.GetOwnerId();
	return os;
}

auto operator==(const KeyHash::HashId& left, const KeyHash::HashId& right) -> bool {
	return left.Get() == right.Get();
}

}
}

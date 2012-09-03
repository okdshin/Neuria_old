#pragma once
//KeyHash:20120903
#include <iostream>
#include <string>


namespace nr
{
class KeyHash{
public:
	class Id{
		public:
			explicit Id(const std::string& val):val(val){}
			auto Get()const -> std::string {
				return this->val;	
			}

		private:
			std::string val;
	};

	class Keyward{
		public:
			explicit Keyward(const std::string& val):val(val){}
			auto Get()const -> std::string {
				return this->val;	
			}

		private:
			std::string val;
	};
	
	auto GetId()const -> Id { return id; }
	auto GetIdStr()const -> std::string { return id.Get(); }
	auto GetKeyward()const -> Keyward { return keyward; }
	auto GetKeywardStr()const -> std::string { return keyward.Get(); }

    KeyHash(const Id& id, const Keyward& keyward) 
		:id(id), keyward(keyward){}

    KeyHash(const Keyward& keyward, const Id& id) 
		:id(id), keyward(keyward){}
private:
	Id id;
	Keyward keyward;

};

auto operator <<(std::ostream& os, const KeyHash::Id& id) -> std::ostream& {
	os << id.Get();
	return os;
}

auto operator <<(std::ostream& os, const KeyHash::Keyward& keyward) -> std::ostream& {
	os << keyward.Get();
	return os;
}

auto operator==(const KeyHash::Id& left, const KeyHash::Id& right) -> bool {
	return left.Get() == right.Get();
}

}


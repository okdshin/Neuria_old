#pragma once
//LabeledSink:20120904
#include <iostream>
#include <string>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

namespace nr{
namespace utl{

class LabeledSink : public boost::iostreams::sink {
public:
	using Stream = boost::iostreams::stream<LabeledSink>;
    LabeledSink(const std::string& label, std::ostream& os=std::cout)
		:label(label), os(os){}

	auto write(const char* s, std::streamsize n) -> std::streamsize {
		os << label << ":";
		this->os.write(s, n);

		return n;
	}

private:
	
	std::string label;
	std::ostream& os;
};

}
}

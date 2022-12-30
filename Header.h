#pragma once
#include <string>
#include <codecvt>
#include <chrono>
#include "unicode.h"
#include "msgpack.hpp"

namespace chr = std::chrono;
using namespace std::literals;

class TestRecord {
	std::u8string name = u8"Wiesenhütter";
	std::string vorname = "Stephan";
	chr::year_month_day geburtstag = 1970y / 05 / 1;

	friend std::wostream& operator<<(std::wostream& os, const TestRecord& rec);
	
public:
	template<class T>
	void pack(T& packer) {
		auto day = static_cast<unsigned int> (this->geburtstag.day());
		auto month = static_cast<unsigned int> (this->geburtstag.month());
		auto year = static_cast<int> (this->geburtstag.year());
		packer(this->name, vorname, day, month, year);
	}
	
};

struct EndCmd {
	const std::string cmd = "END";
	template<class T>
	void pack(T& packer) {
		packer(cmd);
	}
};


std::wostream& operator<<(std::wostream& os, const TestRecord& rec)
{
	std::wcout.imbue(std::locale("en_US.utf8"));
	os << from_utf8<char>(rec.vorname) << " ";
	os << from_utf8<char8_t>(rec.name) << "\n";
	os << "Geboren am " << rec.geburtstag.day() << "." << rec.geburtstag.month() << "." << rec.geburtstag.year() << "\n";
	return os;
}
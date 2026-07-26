#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace prism {

	class PrismReader {
	private:
		std::string filepath;
		std::ifstream ifstream_code;
		std::ostringstream ostringstream_code;
		std::string string_code;

	public:
		PrismReader(std::string filepath);
		~PrismReader();

		std::string_view get();

		void dump();

		bool is_open();
	};

}
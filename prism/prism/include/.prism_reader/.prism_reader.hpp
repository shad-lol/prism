#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "include/compiler_config/compiler_config.hpp"

namespace prism {

	class PrismReader {
	private:
		std::ifstream ifstream_code;
		std::ostringstream ostringstream_code;
		std::string string_code;

	public:
		PrismReader(const CompilerConfig& compilerconfig);
		~PrismReader();

		std::string_view get() { return string_code; };

		bool is_open() { return ifstream_code.is_open(); };
	};

}
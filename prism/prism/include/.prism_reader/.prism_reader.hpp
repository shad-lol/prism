#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "include/lexer/compiler_config.hpp"

namespace prism {

	class PrismReader {
	private:
		std::ifstream ifstream_code;
		std::ostringstream ostringstream_code;
		std::string string_code;

	public:
		PrismReader(std::string filepath, CompilerConfig& compilerconfig);
		~PrismReader();

		std::string_view get();

		void dump();

		bool is_open();
	};

}
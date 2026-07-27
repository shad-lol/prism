#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

#include "compiler_config.hpp"

namespace prism {

	class ArgReader {
	private:
		std::vector<std::string> arguements;
		std::vector<std::string> files;

	public:
		ArgReader(int argc, char* argv[], CompilerConfig& compilerconfig);

		size_t filecount();
		std::string file(int file_id) { return prism::ArgReader::files[file_id]; }
	};

}
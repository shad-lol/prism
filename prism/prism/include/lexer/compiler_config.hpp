#pragma once

#include <cstdint>
#include <string>

namespace prism {

	struct CompilerConfig {
		std::string filepath = "";
		int filecount = 0;
		bool dump_prism = false;
		bool dump_tokens = false;
	};

}
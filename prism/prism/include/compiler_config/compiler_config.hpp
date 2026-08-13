#pragma once

#include <cstdint>
#include <string>

namespace prism {

	struct CompilerConfig {
		std::string filepath = "";
		int filecount = 0;

		bool dump_source = false;
		bool dump_tokens = false;
		bool dump_ast = false;
		bool dump_pir = false;

		bool ansi = true;
		bool utf8 = true;
	};

}
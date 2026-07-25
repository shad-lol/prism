#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "include/error_handler/error_handler.hpp"

namespace prism {

	enum flags : uint8_t {
		build = 1,
		run,
	};

	class ArgReader {
	private:
		std::vector<std::string> active_flags;
		int file_count;

	public:
		ArgReader(int argc, char* argv[]) {
			if (argc < 1 && argv == nullptr) {
				prism::ErrorHandler errorhandler;
				errorhandler.error(ERROR_NO_ARGUEMENTS + '\n');
			}
			active_flags.assign(argv + 1, argv + argc);
		}

		~ArgReader() = default;
	};

}
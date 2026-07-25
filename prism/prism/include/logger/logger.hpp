#pragma once

#include <string>
#include <sstream>

namespace prism {

	struct Logger {
		void coutrgb(const std::string& text);
		void cerrrgb(const std::string& text);
	};

}
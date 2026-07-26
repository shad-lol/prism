#pragma once

#include <string>
#include <vector>

namespace prism {
	class Logger {
	private:
		static std::vector<std::string> color_stack;
		std::string process_color_stack(const std::string& text);
	public:
		void coutrgb(const std::string& text);
		void cerrrgb(const std::string& text);
	};
}

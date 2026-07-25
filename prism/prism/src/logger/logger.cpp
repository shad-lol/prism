#include <iostream>
#include <regex>

#include "include/logger/logger.hpp"

void prism::Logger::coutrgb(const std::string& text) {
	std::string result = "\033[38;2;255;255;255m";
	std::regex color_regex(R"(\[\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\])");
	std::smatch match;

	size_t last_pos = 0;
	auto search_start = text.cbegin();

	while (std::regex_search(search_start, text.cend(), match, color_regex)) {
		result += text.substr(last_pos, match.position());
		result += "\033[38;2;" + match[1].str() + ";" + match[2].str() + ";" + match[3].str() + "m";
		last_pos += match.position() + match.length();
		search_start = match[0].second;
	}

	result += text.substr(last_pos) + "\033[0m";
	std::cout << result;
}

void prism::Logger::cerrrgb(const std::string& text) {
	std::string result = "\033[38;2;255;255;255m";
	std::regex color_regex(R"(\[\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\])");
	std::smatch match;

	size_t last_pos = 0;
	auto search_start = text.cbegin();

	while (std::regex_search(search_start, text.cend(), match, color_regex)) {
		result += text.substr(last_pos, match.position());
		result += "\033[38;2;" + match[1].str() + ";" + match[2].str() + ";" + match[3].str() + "m";
		last_pos += match.position() + match.length();
		search_start = match[0].second;
	}

	result += text.substr(last_pos) + "\033[0m";
	std::cerr << result;
}
#include "include/logger/logger.hpp"
#include <iostream>

std::vector<std::string> prism::Logger::color_stack = { "\033[38;2;240;240;240m" };

std::string prism::Logger::process_color_stack(const std::string& text, bool ansi) {
	std::string result = ansi ? color_stack.back() : "";
	size_t i = 0;
	size_t n = text.size();
	while (i < n) {
		if (text[i] == '[') {
			size_t close_pos = text.find(']', i);
			if (close_pos != std::string::npos) {
				std::string content = text.substr(i + 1, close_pos - i - 1);

				if (!ansi) {
					int r = 0, g = 0, b = 0;
					if (content.empty() || sscanf_s(content.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
						i = close_pos + 1;
						continue;
					}
					result += text.substr(i, close_pos - i + 1);
					i = close_pos + 1;
					continue;
				}

				if (content.empty()) {
					if (color_stack.size() > 1) color_stack.pop_back();
					result += color_stack.back();
				}
				else {
					int r = 0, g = 0, b = 0;
					if (sscanf_s(content.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
						std::string ansi_code = "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
						color_stack.push_back(ansi_code);
						result += ansi_code;
					}
					else {
						result += text.substr(i, close_pos - i + 1);
					}
				}
				i = close_pos + 1;
				continue;
			}
		}
		result += text[i];
		i++;
	}
	return result;
}

void prism::Logger::coutrgb(const std::string& text, bool ansi) {
	std::cout << process_color_stack(text, ansi);
	if (ansi) std::cout << "\033[0m";
	color_stack = { "\033[38;2;240;240;240m" };
}

void prism::Logger::cerrrgb(const std::string& text, bool ansi) {
	std::cerr << process_color_stack(text, ansi);
	if (ansi) std::cerr << "\033[0m";
	std::cerr << std::flush;
	color_stack = { "\033[38;2;240;240;240m" };
	std::exit(EXIT_FAILURE);
}

#include "include/error_handler/error_handler.hpp"
#include "include/logger/logger.hpp"

#include <iomanip>
#include <sstream>

void prism::ErrorHandler::execute_log(uint32_t error_code, const std::vector<std::string>& args) {
	prism::Logger logger;
	uint32_t severity = error_code >> 28;
	bool is_err = (severity >= 0xA);

	std::string sev_txt = "[108, 117, 125]unknown[]";
	switch (severity) {
	case 0x1: sev_txt = "[40, 167, 69]success[]"; break;
	case 0x9: sev_txt = "[33, 150, 243]info[]"; break;
	case 0xA: sev_txt = "[255, 140, 0]warning[]"; break;
	case 0xE: sev_txt = "[220, 53, 69]error[]"; break;
	case 0xF: sev_txt = "[139, 0, 0]fatal error[]"; break;
	}

	std::ostringstream hex_stream;
	hex_stream << "(" << "[100, 181, 246]0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << error_code << "[]): ";
	std::string prefix = sev_txt + hex_stream.str();

	std::string raw = ERROR_MAP[error_code];
	if (raw.empty()) raw = "Unknown error";

	std::string resolved = "";
	size_t arg_idx = 0;
	size_t i = 0;
	size_t n = raw.size();

	while (i < n) {
		if (raw[i] == '$') {
			i++;
			while (i < n && (isalnum(raw[i]) || raw[i] == '_')) i++;

			if (arg_idx < args.size()) {
				resolved += "[234, 222, 0]" + args[arg_idx++] + "[]";
			} else {
				resolved += "[255, 0, 127]If you are seeing this that means the compiler dev broke this log. Please report the [100, 181, 246]blue error code on the left[] at [0, 240, 255]https://github.com/shad-lol/prism/issues[][]";
			}
			continue;
		}
		resolved += raw[i];
		i++;
	}

	std::string final_msg = prefix + resolved + "\n";
	if (is_err) logger.cerrrgb(final_msg);
	else logger.coutrgb(final_msg);
}

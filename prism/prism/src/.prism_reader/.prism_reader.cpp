#include <iostream>
#include <cerrno>
#include <filesystem>

#include "include/.prism_reader/.prism_reader.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/logger/logger.hpp"

prism::PrismReader::PrismReader(std::string filepath) {
	ifstream_code.open(filepath);
	if (!ifstream_code.is_open()) {
		prism::ErrorHandler errorhandler;
		prism::Logger logger;

		if (std::filesystem::is_directory(filepath)) {
			errno = EISDIR;
		}

		if (errno == EISDIR) {
			errorhandler.log(ERROR_IS_A_DIRECTORY);
			logger.cerrrgb("[234, 222, 0]" + filepath);
		}
		else if (!filepath.ends_with(".prism")) {
			errorhandler.log(ERROR_NOT_DOT_PRISM);
			logger.cerrrgb("[234, 222, 0]" + filepath);
		}
		else if (errno == ENOENT) {
			errorhandler.log(ERROR_FILE_NOT_FOUND);
		}
		else if (errno == EACCES) {
			errorhandler.log(ERROR_PERMISSION_DENIED);
		}
		else {
			errorhandler.log(UKNOWN_FILE_ERROR);
			logger.cerrrgb("[234, 222, 0] " + filepath);
		}
	}
	ostringstream_code << ifstream_code.rdbuf();
	string_code = ostringstream_code.str();
}

prism::PrismReader::~PrismReader() {
	if (ifstream_code.is_open()) {
		ifstream_code.close();
	}
}

std::string_view prism::PrismReader::get() {
	return this->string_code;
}

void prism::PrismReader::dump() {
	std::cout << string_code << std::endl;
}

bool prism::PrismReader::is_open() {
	return ifstream_code.is_open();
}
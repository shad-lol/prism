#include <iostream>
#include <cerrno>
#include <filesystem>

#include "include/.prism_reader/.prism_reader.hpp"
#include "include/logger/logger.hpp"
#include "include/error_handler/error_handler.hpp"

prism::PrismReader::PrismReader(std::string filepath) {
	this->filepath = filepath;
	ifstream_code.open(filepath);
	if (!ifstream_code.is_open()) {
		ErrorHandler errorhandler;

		if (errno == ENOENT) {
			errorhandler.log(ERROR_FILE_NOT_FOUND, filepath);
		} else if (errno == EACCES) {
			errorhandler.log(ERROR_PERMISSION_DENIED, filepath);
		} else {
			errorhandler.log(UKNOWN_FILE_ERROR, filepath);
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
	Logger logger;
	logger.coutrgb("[234, 222, 0]" + filepath + ":[]\n");
	std::cout << string_code << "\n\n\n";
}

bool prism::PrismReader::is_open() {
	return ifstream_code.is_open();
}
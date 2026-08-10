#include "include/.prism_reader/.prism_reader.hpp"
#include "include/logger/logger.hpp"
#include "include/error_handler/error_handler.hpp"

#include <iostream>
#include <cerrno>
#include <filesystem>

prism::PrismReader::PrismReader(std::string filepath, CompilerConfig& compilerconfig) {
	ifstream_code.open(filepath);
	if (!ifstream_code.is_open()) {
		ErrorHandler errorhandler;

		if (errno == ENOENT) {
			errorhandler.log(ERROR_FILE_NOT_FOUND, compilerconfig, filepath);
		} else if (errno == EACCES) {
			errorhandler.log(ERROR_PERMISSION_DENIED, compilerconfig, filepath);
		} else {
			errorhandler.log(UKNOWN_FILE_ERROR, compilerconfig, filepath);
		}
	}

	ostringstream_code << ifstream_code.rdbuf();
	string_code = ostringstream_code.str();

	if (compilerconfig.dump_prism) {
		ErrorHandler errorhandler;
		Logger logger;
		errorhandler.log(INFO_PRISM_DUMP, compilerconfig, filepath);
		logger.coutrgb("[78, 201, 176]" + string_code + "\n\n", compilerconfig.ansi);
	}
}

prism::PrismReader::~PrismReader() {
	if (ifstream_code.is_open()) {
		ifstream_code.close();
	}
}
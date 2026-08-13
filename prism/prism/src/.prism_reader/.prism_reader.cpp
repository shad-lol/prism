/*
 *  Copyright 2026 The Prism Authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "include/.prism_reader/.prism_reader.hpp"
#include "include/logger/logger.hpp"
#include "include/error_handler/error_handler.hpp"

#include <iostream>
#include <cerrno>
#include <filesystem>

prism::PrismReader::PrismReader(const CompilerConfig& compilerconfig) {
	ifstream_code.open(compilerconfig.filepath);
	if (!ifstream_code.is_open()) {
		ErrorHandler errorhandler;

		if (errno == ENOENT) {
			errorhandler.log(ERROR_FILE_NOT_FOUND, compilerconfig, compilerconfig.filepath);
		} else if (errno == EACCES) {
			errorhandler.log(ERROR_PERMISSION_DENIED, compilerconfig, compilerconfig.filepath);
		} else {
			errorhandler.log(UKNOWN_FILE_ERROR, compilerconfig, compilerconfig.filepath);
		}
	}

	ostringstream_code << ifstream_code.rdbuf();
	string_code = ostringstream_code.str();

	if (compilerconfig.dump_source) {
		ErrorHandler errorhandler;
		errorhandler.log(INFO_SOURCE_DUMP, compilerconfig, compilerconfig.filepath, "[78, 201, 176]" + string_code + "\n");
	}
}

prism::PrismReader::~PrismReader() {
	if (ifstream_code.is_open()) {
		ifstream_code.close();
	}
}
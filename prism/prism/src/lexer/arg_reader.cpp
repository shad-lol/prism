#include <algorithm>

#include "include/lexer/arg_reader.hpp"
#include "include/error_handler/error_handler.hpp"

prism::ArgReader::ArgReader(int argc, char* argv[]) {
	if (argc < 1 && argv == nullptr) {
		prism::ErrorHandler errorhandler;
		errorhandler.log(ERROR_NO_ARGUEMENTS);
	}

	arguements.assign(argv + 1, argv + argc);

	int i = 0;
	while (i < arguements.size()) {
		if (arguements[i].ends_with(".prism")) {
			files.push_back(arguements[i]);
			arguements.erase(arguements.begin() + i);
		} else {
			i++;
		}
	}
}

size_t prism::ArgReader::filecount() {
	if (files.size() > 1) {
		prism::ErrorHandler errorhandler;
		errorhandler.log(ALERT_MULTIPLE_FILES);
	}
	return files.size();
}

void prism::ArgReader::process() {
	for (int i = 0; i < arguements.size(); i++) {
		if (arguements[i].starts_with("-") && !arguements[i].starts_with("--")) {
			
		}
		
		else if (arguements[i].starts_with("--")) {
			if (arguements[i] == "--output") {

			}
			else if (arguements[i] == "--prism-dump") {
				enable(flags::prism_dump);
			}
		}
		
		else {
			prism::ErrorHandler errorhandler;
			prism::Logger logger;
			errorhandler.log(err::ERROR_INVALID_COMMAND);
			logger.cerrrgb("[234, 222, 0]" + arguements[i]);
		}
	}
}
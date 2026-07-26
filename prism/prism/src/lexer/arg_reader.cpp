#include <filesystem>
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

	for (int i = 0; i < arguements.size(); i++) {
		if (arguements[i].starts_with("-") && !arguements[i].starts_with("--")) {

		}

		else if (arguements[i].starts_with("--")) {
			if (arguements[i] == "--prism-dump") {
				compileconfig.dump_prism = true;
			}
		}

		else if (!arguements[i].starts_with("-") && !arguements[i].starts_with("--")) {
			ErrorHandler errorhandler;

			if (std::filesystem::is_directory(arguements[i])) {
				errno = EISDIR;
			}

			if (errno == EISDIR) {
				errorhandler.log(ERROR_IS_A_DIRECTORY, arguements[i]);
			}
			else if (!arguements[i].ends_with(".prism")) {
				errorhandler.log(ERROR_NOT_DOT_PRISM, arguements[i]);
			} else {
				errorhandler.log(err::ERROR_INVALID_COMMAND, arguements[i]);
			}
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

prism::ArgReader::ConfigValue prism::ArgReader::getconfig_wrapper(ConfigId id) const {
	switch (id) {
		case ConfigId::DumpPrism: return compileconfig.dump_prism;
		default:
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_INVALID_CONFIG_ID);
			return ConfigValue{};
	}
}
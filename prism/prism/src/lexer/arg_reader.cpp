#include <filesystem>
#include <algorithm>

#include "include/lexer/arg_reader.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/lexer/compiler_config.hpp"

prism::ArgReader::ArgReader(int argc, char* argv[], CompilerConfig& compilerconfig) {
	if (argc < 1 && argv == nullptr) {
		ErrorHandler errorhandler;
		errorhandler.log(ERROR_NO_ARGUEMENTS);
	}

	arguements.assign(argv + 1, argv + argc);

	for (int i = 0; i < arguements.size(); ) {
		if (arguements[i].ends_with(".prism")) {
			files.push_back(arguements[i]);
			arguements.erase(arguements.begin() + i);
			compilerconfig.filecount++;
		} else {
			i++;
		}
	}

	if (compilerconfig.filecount > 1) {
		ErrorHandler errorhandler;
		errorhandler.log(ALERT_MULTIPLE_FILES);
	}

	for (int i = 0; i < arguements.size(); i++) {
		if (arguements[i].starts_with("-") && !arguements[i].starts_with("--")) {

		}

		else if (arguements[i].starts_with("--")) {
			if (arguements[i] == "--dump-prism") {
				compilerconfig.dump_prism = true;
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
	
	return files.size();
}
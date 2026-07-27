#include <filesystem>
#include <algorithm>
#include <string>

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
		std::string flag = arguements[i];

		if (flag.starts_with("-") && !flag.starts_with("--")) {
			if (flag == "-doll") {
				compilerconfig.dump_prism = true;
			}
		}

		else if (flag.starts_with("--")) {
			if (flag == "--dump-prism") {
				compilerconfig.dump_prism = true;
			} else if (flag == "--dump-all") {
				compilerconfig.dump_prism = true;
			} else {
				ErrorHandler errorhandler;
				errorhandler.log(err::ERROR_INVALID_COMMAND, flag);
			}
		}

		else if (!flag.starts_with("-") && !flag.starts_with("--")) {
			if (std::filesystem::is_directory(flag)) {
				errno = EISDIR;
			}

			if (errno == EISDIR) {
				ErrorHandler errorhandler;
				errorhandler.log(ERROR_IS_A_DIRECTORY, flag);
			}
			else if (!flag.ends_with(".prism")) {
				ErrorHandler errorhandler;
				errorhandler.log(ERROR_NOT_DOT_PRISM, flag);
			} else {
				ErrorHandler errorhandler;
				errorhandler.log(err::ERROR_INVALID_COMMAND, flag);
			}
		}
	}
}

size_t prism::ArgReader::filecount() {
	
	return files.size();
}
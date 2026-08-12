#include "include/lexer/arg_reader.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/lexer/compiler_config.hpp"

#include <filesystem>
#include <algorithm>
#include <string>

prism::ArgReader::ArgReader(int argc, char* argv[], CompilerConfig& compilerconfig) {
	if (argc < 1 && argv == nullptr) {
		ErrorHandler errorhandler;
		errorhandler.log(ERROR_NO_ARGUEMENTS, compilerconfig);
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
		errorhandler.log(ALERT_MULTIPLE_FILES, compilerconfig);
	}

	for (std::string flag : arguements) {
		if (flag == "-dump-source-off") {
			compilerconfig.dump_source = false;
		}

		else if (flag == "-dump-source-on") {
			compilerconfig.dump_source = true;
		}

		else if (flag == "-dump-tokens-off") {
			compilerconfig.dump_tokens = false;
		}

		else if (flag == "-dump-tokens-on") {
			compilerconfig.dump_tokens = true;
		}

		else if (flag == "-dump-ast-off") {
			compilerconfig.dump_ast = false;
		}

		else if (flag == "-dump-ast-on") {
			compilerconfig.dump_ast = true;
		}

		else if (flag == "-dump-all-off") {
			compilerconfig.dump_source = false;
			compilerconfig.dump_tokens = false;
			compilerconfig.dump_ast = false;
		}

		else if (flag == "-doll" || flag == "-dump-all-on") {
			compilerconfig.dump_source = true;
			compilerconfig.dump_tokens = true;
			compilerconfig.dump_ast = true;
		}

		else if (flag == "-ansi-off" || flag == "-color-off") {
			compilerconfig.ansi = false;
		}

		else if (flag == "-ansi-on" || flag == "-color-on") {
			compilerconfig.ansi = true;
		}

		else if (flag == "-utf8-off") {
			compilerconfig.ansi = false;
		}

		else if (flag == "-utf8-on") {
			compilerconfig.ansi = true;
		}

		else if (flag == "-plain") {
			compilerconfig.ansi = false;
			compilerconfig.utf8 = false;
		}

		else if (flag == "-fancy") {
			compilerconfig.ansi = true;
			compilerconfig.utf8 = true;
		}

		else if (!flag.starts_with("-")) {
			if (std::filesystem::is_directory(flag)) {
				errno = EISDIR;
			}

			if (errno == EISDIR) {
				ErrorHandler errorhandler;
				errorhandler.log(ERROR_IS_A_DIRECTORY, compilerconfig, flag);
			}
			else if (!flag.ends_with(".prism")) {
				ErrorHandler errorhandler;
				errorhandler.log(ERROR_NOT_DOT_PRISM, compilerconfig, flag);
			} else {
				ErrorHandler errorhandler;
				errorhandler.log(err::ERROR_INVALID_COMMAND, compilerconfig, flag);
			}
		}

		else {
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_INVALID_COMMAND, compilerconfig, flag);
		}
	}
}

size_t prism::ArgReader::filecount() {
	
	return files.size();
}
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
		if (flag.starts_with("-") && !flag.starts_with("--")) {
			if (flag.starts_with("-doll")) {
				if (flag[5] == '=') {
					if (flag.ends_with("true") || flag.ends_with("default")) {
						compilerconfig.dump_prism = true;
						compilerconfig.dump_tokens = true;
						compilerconfig.dump_ast = true;
					}
					else if (flag.ends_with("false")) {
						compilerconfig.dump_prism = false;
						compilerconfig.dump_tokens = false;
						compilerconfig.dump_ast = false;
					}
					else {
						ErrorHandler errorhandler;
						errorhandler.log(err::ERROR_INVALID_VALUE_CMD, compilerconfig, flag.substr(6), "-doll");
					}
				}
				else {
					compilerconfig.dump_prism = !compilerconfig.dump_prism;
					compilerconfig.dump_tokens = !compilerconfig.dump_tokens;
					compilerconfig.dump_ast = !compilerconfig.dump_ast;
				}
			}

			else if (flag.starts_with("-utf8")) {
				if (flag[5] == '=') {
					if (flag.ends_with("true") || flag.ends_with("default")) {
						compilerconfig.utf8 = true;
					}
					else if (flag.ends_with("false")) {
						compilerconfig.utf8 = false;
					}
					else {
						ErrorHandler errorhandler;
						errorhandler.log(err::ERROR_INVALID_VALUE_CMD, compilerconfig, flag.substr(6), "-utf8");
					}
				}
				else compilerconfig.utf8 = !compilerconfig.utf8;
			}

			else if (flag.starts_with("-ansi")) {
				if (flag[5] == '=') {
					if (flag.ends_with("true") || flag.ends_with("default")) {
						compilerconfig.ansi = true;
					}
					else if (flag.ends_with("false")) {
						compilerconfig.ansi = false;
					}
					else {
						ErrorHandler errorhandler;
						errorhandler.log(err::ERROR_INVALID_VALUE_CMD, compilerconfig, flag.substr(6), "-ansi");
					}
				}
				else compilerconfig.ansi = !compilerconfig.ansi;
			}

			else if (flag.starts_with("-plain")) {
				if (flag[6] == '=') {
					if (flag.ends_with("true")) {
						compilerconfig.ansi = false;
						compilerconfig.utf8 = false;
					}
					else if (flag.ends_with("false")) {
						compilerconfig.ansi = true;
						compilerconfig.utf8 = true;
					}
					else if (flag.ends_with("default")) {
						compilerconfig.ansi = true;
						compilerconfig.utf8 = true;
					}
					else {
						ErrorHandler errorhandler;
						errorhandler.log(err::ERROR_INVALID_VALUE_CMD, compilerconfig, flag.substr(7), "-plain");
					}
				}
				else {
					compilerconfig.ansi = false;
					compilerconfig.utf8 = false;
				}int32_t
			}
			else if (flag.starts_with("-fancy")) {
				if (flag[6] == '=') {
					if (flag.ends_with("true") || flag.ends_with("default")) {
						compilerconfig.ansi = true;
						compilerconfig.utf8 = true;
					}
					else if (flag.ends_with("false")) {
						compilerconfig.ansi = false;
					}
					else {
						ErrorHandler errorhandler;
						errorhandler.log(err::ERROR_INVALID_VALUE_CMD, compilerconfig, flag.substr(7), "-fancy");
					}
				}
				else {
					compilerconfig.ansi = true;
					compilerconfig.utf8 = true;
				}
			}

			else {
				ErrorHandler errorhandler;
				errorhandler.log(err::ERROR_INVALID_COMMAND, compilerconfig, flag);
			}
		}

		else if (flag.starts_with("--")) {
			if (flag == "--dump-prism") {
				compilerconfig.dump_prism = true;
			}
			else if (flag == "--dump-tokens") {
				compilerconfig.dump_tokens = true;
			}
			else if (flag == "--dump-ast") {
				compilerconfig.dump_ast = true;
			}
			else if (flag == "--dump-all") {
				compilerconfig.dump_prism = true;
				compilerconfig.dump_tokens = true;
				compilerconfig.dump_ast = true;
			}

			else {
				ErrorHandler errorhandler;
				errorhandler.log(err::ERROR_INVALID_COMMAND, compilerconfig, flag);
			}
		}

		else if (!flag.starts_with("-") && !flag.starts_with("--")) {
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
	}
}

size_t prism::ArgReader::filecount() {
	
	return files.size();
}
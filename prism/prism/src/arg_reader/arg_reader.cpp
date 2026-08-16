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

#include "include/arg_reader/arg_reader.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/compiler_config/compiler_config.hpp"

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

		else if (flag == "-dump-pir-off") {
			compilerconfig.dump_pir = false;
		}

		else if (flag == "-dump-pir-on") {
			compilerconfig.dump_pir = true;
		}

		else if (flag == "-dump-C-off") {
			compilerconfig.dump_c = false;
		}

		else if (flag == "-dump-C-on") {
			compilerconfig.dump_c = true;
		}

		else if (flag == "-dump-LLVM-off") {
			compilerconfig.dump_llvm = false;
		}

		else if (flag == "-dump-LLVM-on") {
			compilerconfig.dump_llvm = true;
		}

		else if (flag == "-dump-cmd-off") {
			compilerconfig.dump_cmd = false;
		}

		else if (flag == "-dump-cmd-on") {
			compilerconfig.dump_cmd = true;
		}

		else if (flag == "-dump-all-off") {
			compilerconfig.dump_source = false;
			compilerconfig.dump_tokens = false;
			compilerconfig.dump_ast = false;
			compilerconfig.dump_pir = false;
			compilerconfig.dump_c = false;
			compilerconfig.dump_cmd = false;
		}

		else if (flag == "-doll" || flag == "-dump-all-on") {
			compilerconfig.dump_source = true;
			compilerconfig.dump_tokens = true;
			compilerconfig.dump_ast = true;
			compilerconfig.dump_pir = true;
			compilerconfig.dump_c = true;
			compilerconfig.dump_cmd = true;
		}

		else if (flag == "-generate-exe-off") {
			compilerconfig.generate_exe = false;
		}

		else if (flag == "-generate-exe-on" || flag == "-generate-exe") {
			compilerconfig.generate_exe = true;
		}

		else if (flag == "-generate-obj-off") {
			compilerconfig.generate_obj = false;
		}

		else if (flag == "-generate-obj-on" || flag == "-generate-obj") {
			compilerconfig.generate_obj = true;
		}

		else if (flag == "-O0") {
			compilerconfig.O0 = true;
			compilerconfig.O1 = false;
			compilerconfig.O2 = false;
			compilerconfig.O3 = false;
			compilerconfig.Os = false;
			compilerconfig.Oz = false;
		}

		else if (flag == "-O1") {
			compilerconfig.O0 = false;
			compilerconfig.O1 = true;
			compilerconfig.O2 = false;
			compilerconfig.O3 = false;
			compilerconfig.Os = false;
			compilerconfig.Oz = false;
		}

		else if (flag == "-O2") {
			compilerconfig.O0 = false;
			compilerconfig.O1 = false;
			compilerconfig.O2 = true;
			compilerconfig.O3 = false;
			compilerconfig.Os = false;
			compilerconfig.Oz = false;
		}

		else if (flag == "-O3") {
			compilerconfig.O0 = false;
			compilerconfig.O1 = false;
			compilerconfig.O2 = false;
			compilerconfig.O3 = true;
			compilerconfig.Os = false;
			compilerconfig.Oz = false;
		}

		else if (flag == "-Os") {
			compilerconfig.O0 = false;
			compilerconfig.O1 = false;
			compilerconfig.O2 = false;
			compilerconfig.O3 = false;
			compilerconfig.Os = true;
			compilerconfig.Oz = false;
		}

		else if (flag == "-Oz") {
			compilerconfig.O0 = false;
			compilerconfig.O1 = false;
			compilerconfig.O2 = false;
			compilerconfig.O3 = false;
			compilerconfig.Os = false;
			compilerconfig.Oz = true;
		}

		else if (flag == "-C") {
			compilerconfig.C = true;
			compilerconfig.LLVM = false;
		}

		else if (flag == "-LLVM") {
			compilerconfig.C = false;
			compilerconfig.LLVM = true;
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

		else if (!flag.starts_with('-')) {
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
			}
		} else {
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_INVALID_COMMAND, compilerconfig, flag);
		}
	}
}

size_t prism::ArgReader::filecount() {
	
	return files.size();
}
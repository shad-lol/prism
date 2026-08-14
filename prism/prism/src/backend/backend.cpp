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

#include "include/backend/backend.hpp"

#include "include/error_handler/error_handler.hpp"

prism::Backend::Backend(const std::string& ccode, const CompilerConfig& compilerconfig) {
	if (compilerconfig.C) {
		std::string opt;

		if (compilerconfig.O0) opt = "-O0";
		if (compilerconfig.O1) opt = "-O1";
		if (compilerconfig.O2) opt = "-O2";
		if (compilerconfig.O3) opt = "-O3";
		if (compilerconfig.Os) opt = "-Os";
		if (compilerconfig.Oz) opt = "-Oz";

		std::filesystem::path exe_path = compilerconfig.filedir / (compilerconfig.appname + ".exe");
		std::string cmd;

		if (compilerconfig.generate_obj) {
			std::filesystem::path obj_path = compilerconfig.filedir / (compilerconfig.appname + ".obj");
			cmd = "clang -x c " + opt + " -c - -o \"" + obj_path.string() + "\" && clang \"" + obj_path.string() + "\" -o \"" + exe_path.string() + "\"";
		}
		else cmd = "clang -x c " + opt + " - -o \"" + exe_path.string() + "\"";

		if (compilerconfig.dump_cmd) {
			ErrorHandler errorhandler;
			errorhandler.log(err::INFO_CMD_DUMP, compilerconfig, compilerconfig.filepath, cmd);
		}

		FILE* pipe = _popen((cmd).c_str(), "w");

		if (pipe) {
			std::fwrite(ccode.c_str(), 1, ccode.length(), pipe);
			_pclose(pipe);
		} else {
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_NO_CLANG, compilerconfig);
		}

	}
}
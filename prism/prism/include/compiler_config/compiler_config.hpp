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

#pragma once

#include <cstdint>
#include <string>
#include <filesystem>

namespace prism {

	struct CompilerConfig {
		std::string appname;

		std::filesystem::path filepath;
		std::filesystem::path filedir;
		int filecount = 0;

		bool dump_source = false;
		bool dump_tokens = false;
		bool dump_ast = false;
		bool dump_pir = false;
		bool dump_c = false;
		bool dump_llvm = false;
		bool dump_cmd = false;

		bool ansi = true;
		bool utf8 = true;

		bool O0 = true;
		bool O1 = false;
		bool O2 = false;
		bool O3 = false;
		bool Os = false;
		bool Oz = false;

		bool generate_exe = true;
		bool generate_obj = false;

		bool C = true;
		bool LLVM = false;
	};

}
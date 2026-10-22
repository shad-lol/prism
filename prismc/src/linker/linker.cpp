
/*   Copyright 2026 Prism Compiler Authors
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <global.hpp>

#include <filesystem>
#include <cstdlib>

namespace prismc {

	PRISMC_API uint16_t link(std::vector<std::filesystem::path> obj_files, std::filesystem::path output_path) {
		std::string cmd;
		cmd.reserve(512 + (obj_files.size() * 64));

		#if defined(_WIN32)
			cmd = "lld-link.exe";

			for (auto& obj : obj_files) {
				if (obj.extension() != ".obj")
					obj += ".obj";
				cmd += " \"" + obj.string() + "\"";
			}

			if (output_path.extension() != ".exe")
				output_path.replace_extension(".exe");

			cmd += " /OUT:\"" + output_path.string() + "\"";

			cmd +=  " /LIBPATH:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\ucrt\\x64\""
					" /LIBPATH:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\um\\x64\""
					" /LIBPATH:\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.36.32532\\lib\\x64\"";

			cmd += " msvcrt.lib kernel32.lib ucrt.lib /subsystem:console /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:libucrt.lib";

		#elif defined(__linux__)
			cmd = "ld.lld";

			for (auto& obj : obj_files) {
				if (obj.extension() != ".obj")
					obj += ".obj";
				cmd += " \"" + obj.string() + "\"";
			}

			cmd += " -o \"" + output_path.string() + "\" -lc -lm --dynamic-linker=/lib64/ld-linux-x86-64.so.2";

		#elif defined(__APPLE__)
			cmd = "ld.lld -flavor darwin";

			for (auto& obj : obj_files) {
				if (obj.extension() != ".obj")
					obj += ".obj";
				cmd += " \"" + obj.string() + "\"";
			}

			cmd += " -o \"" + output_path.string() + "\" -lSystem";
		#endif

		int res = std::system(cmd.c_str());

		if (res != 0)
			return diag.log(err::ErrLinkError);

		return err::Success;
	}

}
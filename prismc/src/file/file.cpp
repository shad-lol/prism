
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

#include <file/file.hpp>

#include <fstream>

std::expected<void, uint16_t> prismc::File::load(const std::filesystem::path& path) {
	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		if (errno == ENOENT)      return std::unexpected(diag.log(err::ErrFileNotFound, path.string()));
		else if (errno == EACCES) return std::unexpected(diag.log(err::ErrAccessDenied, path.string()));
		else                      return std::unexpected(diag.log(err::ErrUnknownFileError, path.string()));
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();

	code = buffer.str();
}
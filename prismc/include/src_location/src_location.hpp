
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

#pragma once

#include <global.hpp>
#include <filesystem>

namespace prismc {

	struct SourceLocation {

		const std::filesystem::path* filepath;
		uint32_t pos = 0;
		uint16_t line = 1;
		uint16_t column = 1;

		SourceLocation() = default;

		SourceLocation(const std::filesystem::path* filepath, size_t pos, size_t line, size_t column)
			: filepath(filepath), pos(pos), line(line), column(column) {
		}

		std::string as_string() const {
			if (!filepath) {
				return std::format("unknown:{}:{}", line, column);
			}
			return std::format("{}:{}:{}", filepath->string(), line, column);
		}

	};

}
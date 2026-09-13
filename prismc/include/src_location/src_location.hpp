
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

namespace prismc {

	struct SourceLocation {

		const std::string* filename;
		uint32_t pos = 0;
		uint16_t line = 1;
		uint16_t column = 1;

		SourceLocation() = default;

		SourceLocation(const std::string* filename, size_t pos, size_t line, size_t column)
			: filename(filename), pos(pos), line(line), column(column) {
		}

		std::string to_string() const {
			if (!filename) return "unknown:" + std::to_string(line) + ":" + std::to_string(column);
			return *filename + ":" + std::to_string(line) + ":" + std::to_string(column);
		}

	};

}

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
#include <string>
#include <expected>

namespace prismc {

	class PRISMC_API File {

	public:
		File() = default;

		std::expected<void, uint16_t> load(const std::filesystem::path& path);

		[[nodiscard]] constexpr const std::filesystem::path& get_path() const noexcept { return path; }
		[[nodiscard]] constexpr const std::string& get_code() const noexcept { return code; }
		constexpr void set_path(const std::filesystem::path& path) noexcept { this->path = path; }
		constexpr void set_code(const std::string& code) noexcept { this->code = code; }

	private:
		std::filesystem::path path;
		std::string code;

	};

}
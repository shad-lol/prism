
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

#include "token.hpp"

#include <string>

namespace prismc {

	class PRISMC_API Lexer {

	public:
		Lexer() = default;

		std::vector<Token> lex(const std::string_view& code);

		[[nodiscard]] constexpr const std::string& get_code() const noexcept { return code; }
		[[nodiscard]] constexpr const size_t& get_pos() const noexcept { return pos; }
		[[nodiscard]] constexpr const size_t& get_line() const noexcept { return line; }
		[[nodiscard]] constexpr const size_t& get_column() const noexcept { return column; }

	private:
		std::string code;

		size_t pos = 0;
		size_t line = 1;
		size_t column = 1;

	};

}
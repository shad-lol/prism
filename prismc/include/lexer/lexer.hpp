
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
#include <file/file.hpp>

#include <string>
#include <vector>

namespace prismc {

	class PRISMC_API Lexer {

	public:
		Lexer() = default;

		std::expected<std::vector<Token>, uint16_t> lex(const File& file);

		Token lex_identifier_or_keyword();
		std::expected<Token, bool> lex_string();
		Token lex_number();

		bool skip();

		void skip_whitespace();
		void skip_comment();
		bool skip_multiline_comment();

		char peek() const;
		char next() const;

		char advance();

	private:
		std::string_view code;
		SourceLocation location;

		inline static const std::unordered_map<std::string_view, TokenType> keywords = {
			{"let", TokenType::KwLet},
			{"set", TokenType::KwSet},
			{"func", TokenType::KwFunc},
			{"entry", TokenType::KwEntry},
			{"return", TokenType::KwReturn},
			{"if", TokenType::KwIf},
			{"else", TokenType::KwElse},
			{"while", TokenType::KwWhile},
			{"struct", TokenType::KwStruct},
			{"import", TokenType::KwImport},
			{"true", TokenType::BoolLiteral},
			{"false", TokenType::BoolLiteral}
		};

	};

}
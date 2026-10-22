
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
#include <lexer/unicode_char.hpp>

#include <string>
#include <vector>

namespace prismc {

	class PRISMC_API Lexer {

	public:
		std::expected<std::vector<Token>, uint16_t> tokenize(const File& file);

	private:
		Token tokenize_identifier_or_keyword();
		Token tokenize_operator();
		std::expected<Token, bool> tokenize_string();
		Token tokenize_number();

		std::string_view code;
		SourceLocation location;

		[[nodiscard]] UnicodeChar peek_unicode() const;
		[[nodiscard]] bool is_operator_char(uint32_t cp) const;
		[[nodiscard]] bool is_identifier_start_char(uint32_t cp) const;
		[[nodiscard]] bool is_identifier_continue_char(uint32_t cp) const;

		bool can_insert_newline(const std::vector<Token>& tokens) const;

		bool skip();

		void skip_whitespace();
		void skip_comment();
		bool skip_multiline_comment();

		char peek() const;
		char next() const;
		char advance();

		inline static const std::unordered_map<std::string_view, TokenType> keywords = {
			{"let", TokenType::KwLet},
			{"var", TokenType::KwVar},
			{"func", TokenType::KwFunc},
			{"struct", TokenType::KwStruct},
			{"entry", TokenType::KwEntry},
			{"return", TokenType::KwReturn},
			{"if", TokenType::KwIf},
			{"else", TokenType::KwElse},
			{"while", TokenType::KwWhile},
			{"import", TokenType::KwImport},
			{"public", TokenType::KwPublic},
			{"private", TokenType::KwPrivate},
			{"true", TokenType::BoolLiteral},
			{"false", TokenType::BoolLiteral}
		};

	};

}
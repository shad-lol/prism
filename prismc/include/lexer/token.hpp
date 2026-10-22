
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

#include <src_location/src_location.hpp>
#include <file/file.hpp>

#include <string>
#include <string_view>
#include <variant>

namespace prismc {

	enum class TokenType : uint8_t {

		EoF = 0,
		Invalid,

		Identifier,
		IntLiteral,
		FloatLiteral,
		StringLiteral,
		BoolLiteral,

		Operator,

		Assign,       // =
		Colon,        // :
		NewLine,      // ⏎ (\n)
		Comma,        // ,
		Dot,          // .

		OpenParen,    // (
		CloseParen,   // )
		OpenBrace,    // {
		CloseBrace,   // }
		OpenBracket,  // [
		CloseBracket, // ]

		KwLet,
		KwVar,
		KwFunc,
		KwStruct,
		KwEntry,
		KwReturn,
		KwIf,
		KwElse,
		KwWhile,
		KwImport,
		KwPublic,
		KwPrivate

	};

	constexpr std::string_view as_string(TokenType type);

	struct PRISMC_API Token {

		TokenType type;
		uint8_t length;
		SourceLocation location;

		[[nodiscard]] bool is(TokenType type) const;
		[[nodiscard]] bool is_one_of(std::initializer_list<TokenType> types) const;

		[[nodiscard]] std::expected<std::string, uint16_t> as_string() const;
		[[nodiscard]] std::string as_string(std::string_view code) const;

		[[nodiscard]] std::expected<std::string, uint16_t> lexeme() const;
		[[nodiscard]] std::string lexeme(std::string_view code) const;

	};

}
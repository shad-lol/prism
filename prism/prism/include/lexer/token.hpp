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

#include <string>
#include <variant>

namespace prism {

	enum class TokenType {
		EOFILE,

		KEYWORD_ENTRY,
		KEYWORD_RETURN,

		IDENTIFIER,

		LBRACE,
		RBRACE,

		LPAREN,
		RPAREN,

		SEMICOLON,

		LITERAL_INT,
		LITERAL_LL,
		LITERAL_FLOAT,
		LITERAL_DOUBLE,

		ROOT,
		FUNC,
		FUNC_ARGUEMENTS,
		FUNC_BODY,

		INVALID,
		UNKNOWN,
	};

	struct Token {
		std::string lexeme;
		TokenType type;
		std::variant<std::monostate, int, long long, float, double> value;
	};

}
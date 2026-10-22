
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

#include <lexer/token.hpp>

namespace prismc {

	constexpr std::string_view as_string(TokenType type) {
		switch (type) {

			case TokenType::EoF: return "EoF";
			case TokenType::Invalid: return "Invalid";

			case TokenType::Identifier: return "Identifier";
			case TokenType::IntLiteral: return "IntLiteral";
			case TokenType::FloatLiteral: return "FloatLiteral";
			case TokenType::StringLiteral: return "StringLiteral";
			case TokenType::BoolLiteral: return "BoolLiteral";

			case TokenType::Operator: return "Operator";

			case TokenType::Assign: return "Assign";
			case TokenType::Colon: return "Colon";
			case TokenType::NewLine: return "NewLine";
			case TokenType::Comma: return "Comma";
			case TokenType::Dot: return "Dot";

			case TokenType::OpenParen: return "OpenParen";
			case TokenType::CloseParen: return "CloseParen";
			case TokenType::OpenBrace: return "OpenBrace";
			case TokenType::CloseBrace: return "CloseBrace";
			case TokenType::OpenBracket: return "OpenBracket";
			case TokenType::CloseBracket: return "CloseBracket";

			case TokenType::KwLet: return "KwLet";
			case TokenType::KwVar: return "KwVar";
			case TokenType::KwFunc: return "KwFunc";
			case TokenType::KwStruct: return "KwStruct";
			case TokenType::KwEntry: return "KwEntry";
			case TokenType::KwReturn: return "KwReturn";
			case TokenType::KwIf: return "KwIf";
			case TokenType::KwElse: return "KwElse";
			case TokenType::KwWhile: return "KwWhile";
			case TokenType::KwImport: return "KwImport";
			case TokenType::KwPublic: return "KwPublic";
			case TokenType::KwPrivate: return "KwPrivate";

		}

		return "Unknown";
	}

	bool Token::is(TokenType type) const {
		if (this->type == type) return true;
		return false;
	}

	bool Token::is_one_of(std::initializer_list<TokenType> types) const {
		for (auto type : types) {
			if (this->type == type) return true;
		}
		return false;
	}

	std::expected<std::string, uint16_t> Token::as_string() const {
		File file;
		uint16_t res = file.load(*location.path);
		if (res != err::Success) return std::unexpected(res);

		std::string code = file.code;
		std::string_view lexeme(code.data() + location.pos, length);

		if (type == TokenType::NewLine)
			lexeme = "⏎";

		return  std::format("{}: {} '{}'",
				location.as_string(),
				prismc::as_string(type),
				lexeme);
	}

	std::string Token::as_string(std::string_view code) const {
		std::string_view lexeme(code.data() + location.pos, length);

		if (type == TokenType::NewLine) {
			#ifdef _WIN32
				if (GetConsoleOutputCP() == 65001)
					lexeme = "⏎";

				else lexeme = "\\n";
			#else
				lexemme = "⏎";
			#endif
		}

		return  std::format("{}: {} '{}'",
				location.as_string(),
				prismc::as_string(type),
				lexeme);
	}

	std::expected<std::string, uint16_t> Token::lexeme() const {
		File file;
		uint16_t res = file.load(*location.path);
		if (res != err::Success) return std::unexpected(res);
		std::string code = file.code;

		return std::string(code.data() + location.pos, length);
	}

	std::string Token::lexeme(std::string_view code) const {
		return std::string(code.data() + location.pos, length);
	}

}
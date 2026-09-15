
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

			case TokenType::Assign: return "Assign";
			case TokenType::Plus: return "Plus";
			case TokenType::Minus: return "Minus";
			case TokenType::Asterisk: return "Asterisk";
			case TokenType::Slash: return "Slash";
			case TokenType::Percent: return "Percent";

			case TokenType::Equal: return "Equal";
			case TokenType::NotEqual: return "NotEqual";
			case TokenType::LessThan: return "LessThan";
			case TokenType::LessEqual: return "LessEqual";
			case TokenType::GreaterThan: return "GreaterThan";
			case TokenType::GreaterEqual: return "GreaterEqual";

			case TokenType::Amp: return "Amp";
			case TokenType::Pipe: return "Pipe";
			case TokenType::Caret: return "Caret";
			case TokenType::Tilde: return "Tilde";
			case TokenType::LeftShift: return "LeftShift";
			case TokenType::RightShift: return "RightShift";

			case TokenType::AmpAmp: return "AmpAmp";
			case TokenType::PipePipe: return "PipePipe";
			case TokenType::Exclamation: return "Exclamation";

			case TokenType::Colon: return "Colon";
			case TokenType::Semicolon: return "Semicolon";
			case TokenType::Comma: return "Comma";
			case TokenType::Dot: return "Dot";

			case TokenType::OpenParen: return "OpenParen";
			case TokenType::CloseParen: return "CloseParen";
			case TokenType::OpenBrace: return "OpenBrace";
			case TokenType::CloseBrace: return "CloseBrace";
			case TokenType::OpenBracket: return "OpenBracket";
			case TokenType::CloseBracket: return "CloseBracket";

			case TokenType::KwLet: return "KwLet";
			case TokenType::KwSet: return "KwSet";
			case TokenType::KwFunc: return "KwFunc";
			case TokenType::KwEntry: return "KwEntry";
			case TokenType::KwReturn: return "KwReturn";
			case TokenType::KwIf: return "KwIf";
			case TokenType::KwElse: return "KwElse";
			case TokenType::KwWhile: return "KwWhile";
			case TokenType::KwStruct: return "KwStruct";
			case TokenType::KwImport: return "KwImport";

		}

		return "Unknown";
	}

	bool Token::is_one_of(std::initializer_list<TokenType> types) const {
		for (auto t : types) {
			if (type == t) return true;
		}
		return false;
	}

	std::expected<std::string, uint16_t> Token::as_string() const {
		File file;
		auto res = file.load(*location.filepath);
		if (!res) return std::unexpected(res.error());

		std::string code = file.get_code();
		std::string_view lexeme(code.data() + location.pos, length);

		return  std::format("{} '{}' @ {}",
				prismc::as_string(type),
				lexeme,
				location.as_string());
	}

	std::string Token::as_string(std::string_view code) const {
		std::string_view lexeme(code.data() + location.pos, length);

		return  std::format("{} '{}' @ {}",
				prismc::as_string(type),
				lexeme,
				location.as_string());
	}

}
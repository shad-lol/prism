
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

#include <lexer/lexer.hpp>

namespace prismc {

	std::expected<std::vector<Token>, uint16_t> Lexer::tokenize(const File& file) {
		code = file.code;
		location.path = &file.path;

		std::vector<Token> tokens;

		tokens.reserve(code.length() / 4);

		while (location.pos < code.length()) {
			SourceLocation comment_start = location;
			if (!skip())
				return std::unexpected(diag.log(err::SyntaxUnclosedComment, comment_start.as_string()));

			if (location.pos >= code.length())
				break;

			if (peek() == '\n') {
				SourceLocation newline_start = location;
				if (can_insert_newline(tokens))
					tokens.push_back(Token{ TokenType::NewLine, 1, newline_start });

				advance();
				continue;
			}

			SourceLocation token_start = location;
			uint32_t start_pos = location.pos;

			UnicodeChar uc = peek_unicode();
			char c = static_cast<char>(uc.value);

			if (c == '"') {
				auto res = tokenize_string();
				if (!res)
					return std::unexpected(diag.log(err::SyntaxUnclosedStringLiteral, token_start.as_string()));

				tokens.push_back(res.value());
				continue;
			}

			if (c >= '0' && c <= '9') {
				tokens.push_back(tokenize_number());
				continue;
			}

			if (is_identifier_start_char(uc.value)) {
				tokens.push_back(tokenize_identifier_or_keyword());
				continue;
			}

			if (is_operator_char(uc.value)) {
				tokens.push_back(tokenize_operator());
				continue;
			}

			advance();
			TokenType type = TokenType::Invalid;
			
			switch (c) {
				case ':': type = TokenType::Colon; break;
				case ',': type = TokenType::Comma; break;
				case '.': type = TokenType::Dot; break;
				case '(': type = TokenType::OpenParen; break;
				case ')': type = TokenType::CloseParen; break;
				case '{': type = TokenType::OpenBrace; break;
				case '}': type = TokenType::CloseBrace; break;
				case '[': type = TokenType::OpenBracket; break;
				case ']': type = TokenType::CloseBracket; break;
				default:  type = TokenType::Invalid; break;
			}

			if (type == TokenType::Invalid)
				return std::unexpected(diag.log(err::SyntaxInvalidToken, token_start.as_string()));

			uint8_t length = static_cast<uint8_t>(location.pos - start_pos);
			tokens.push_back(Token{ type, length, token_start });
		}

		tokens.push_back(Token{ TokenType::EoF, 0, location });

		return tokens;
	}

	UnicodeChar Lexer::peek_unicode() const {
		if (location.pos >= code.length()) return { 0, 0 };

		uint8_t b1 = static_cast<uint8_t>(code[location.pos]);

		if (b1 < 0x80) return { b1, 1 };

		if ((b1 & 0xE0) == 0xC0 && location.pos + 1 < code.length()) {
			uint8_t b2 = static_cast<uint8_t>(code[location.pos + 1]);
			uint32_t cp = ((b1 & 0x1F) << 6) | (b2 & 0x3F);
			return { cp, 2 };
		}

		if ((b1 & 0xF0) == 0xE0 && location.pos + 2 < code.length()) {
			uint8_t b2 = static_cast<uint8_t>(code[location.pos + 1]);
			uint8_t b3 = static_cast<uint8_t>(code[location.pos + 2]);
			uint32_t cp = ((b1 & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
			return { cp, 3 };
		}

		if ((b1 & 0xF8) == 0xF0 && location.pos + 3 < code.length()) {
			uint8_t b2 = static_cast<uint8_t>(code[location.pos + 1]);
			uint8_t b3 = static_cast<uint8_t>(code[location.pos + 2]);
			uint8_t b4 = static_cast<uint8_t>(code[location.pos + 3]);
			uint32_t cp = ((b1 & 0x07) << 18) | ((b2 & 0x3F) << 12) | ((b3 & 0x3F) << 6) | (b4 & 0x3F);
			return { cp, 4 };
		}

		return { b1, 1 };
	}

	bool Lexer::is_operator_char(uint32_t cp) const {
		if (cp < 0x80) {
			switch (cp) {
				case '+': case '-': case '*': case '/': case '%':
				case '=': case '!': case '<': case '>': case '&':
				case '|': case '^': case '~': case '?':
					return true;
				default:
					return false;
			}
		}

		// Unicode mathematic operators
		if (cp >= 0x2200 && cp <= 0x22FF) return true;

		// Unicode arrows
		if (cp >= 0x2190 && cp <= 0x21FF) return true;

		// Addition unicode arrows and math operators
		if (cp >= 0x2900 && cp <= 0x29FF) return true;
		if (cp >= 0x27C0 && cp <= 0x27EF) return true;

		// Technical and geometrical symbols
		if (cp >= 0x2300 && cp <= 0x23FF) return true;
		if (cp >= 0x25A0 && cp <= 0x25FF) return true;

		return false;
	}

	bool Lexer::is_identifier_start_char(uint32_t cp) const {
		if (cp < 0x80)
			return std::isalpha(static_cast<unsigned char>(cp)) || cp == '_' || cp == '@';
		return !is_operator_char(cp);
	}

	bool Lexer::is_identifier_continue_char(uint32_t cp) const {
		if (cp < 0x80)
			return std::isalnum(static_cast<unsigned char>(cp)) || cp == '_';
		return !is_operator_char(cp);
	}

	Token Lexer::tokenize_identifier_or_keyword() {
		SourceLocation start_loc = location;
		uint32_t start_pos = location.pos;

		advance();

		while (location.pos < code.length()) {
			unsigned char c = static_cast<unsigned char>(peek());

			if (std::isalnum(c) || c == '_')
				advance();
			else if (c >= 0x80)
				advance();
			else break;
		}

		std::string_view text = code.substr(start_pos, location.pos - start_pos);

		TokenType type = TokenType::Identifier;
		if (auto it = keywords.find(text); it != keywords.end()) {
			type = it->second;
		}

		uint8_t length = static_cast<uint8_t>(location.pos - start_pos);
		return Token{ type, length, start_loc };
	}

	Token Lexer::tokenize_operator() {
		SourceLocation start_loc = location;
		uint32_t start_pos = location.pos;

		while (location.pos < code.length()) {
			UnicodeChar uc = peek_unicode();

			if (is_operator_char(uc.value)) {

				if (uc.value == '=' && (location.pos == start_pos)) {
					uint32_t next_pos = location.pos + uc.bytes;
					if (next_pos < code.length()) {
						char next_char = code[next_pos];
						if (next_char == ' ' || next_char == '\t' || next_char == '\r' || next_char == '\n') {
							for (uint8_t i = 0; i < uc.bytes; i++) advance();
							return Token{ TokenType::Assign, 1, start_loc };
						}
					}
				}

				for (uint8_t i = 0; i < uc.bytes; i++) advance();
			}
			else break;
		}

		uint8_t length = static_cast<uint8_t>(location.pos - start_pos);
		return Token{ TokenType::Operator, length, start_loc };
	}

	std::expected<Token, bool> Lexer::tokenize_string() {
		SourceLocation start_loc = location;

		advance();

		while (peek() != '"' && peek() != '\0')
			advance();

		if (peek() == '"') {
			advance();

			uint8_t length = static_cast<uint8_t>(location.pos - start_loc.pos);
			return Token{ TokenType::StringLiteral, length, start_loc };
		}

		return std::unexpected(false);
	}

	Token Lexer::tokenize_number() {
		SourceLocation start_loc = location;

		while (std::isdigit(static_cast<unsigned char>(peek())))
			advance();

		TokenType type = TokenType::IntLiteral;

		if (peek() == '.' && std::isdigit(static_cast<unsigned char>(next()))) {
			type = TokenType::FloatLiteral;
			advance();

			while (std::isdigit(static_cast<unsigned char>(peek())))
				advance();
		}

		uint8_t length = static_cast<uint8_t>(location.pos - start_loc.pos);
		return Token{ type, length, start_loc };
	}

	bool Lexer::can_insert_newline(const std::vector<Token>& tokens) const {
		if (tokens.empty()) return false;

		TokenType last_type = tokens.back().type;
		switch (last_type) {
			case TokenType::Identifier:
			case TokenType::Operator:
			case TokenType::IntLiteral:
			case TokenType::FloatLiteral:
			case TokenType::StringLiteral:
			case TokenType::BoolLiteral:
			case TokenType::KwReturn:
			case TokenType::CloseParen:
			case TokenType::CloseBrace:
			case TokenType::CloseBracket:
				return true;

			default:
				return false;
		}
	}


	bool Lexer::skip() {
		uint32_t last_pos = location.pos;

		while (location.pos < code.length()) {
			skip_whitespace();
			skip_comment();

			if (!skip_multiline_comment())
				return false;

			if (location.pos == last_pos)
				break;
			last_pos = location.pos;
		}
		return true;
	}

	void Lexer::skip_whitespace() {
		while (location.pos < code.length()) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\r')
				advance();
			else break;
		}
	}

	void Lexer::skip_comment() {
		if (peek() == '/' && next() == '/')
			while (peek() != '\n' && peek() != '\0')
				advance();
	}

	bool Lexer::skip_multiline_comment() {
		if (peek() == '/' && next() == '*') {
			advance();
			advance();

			while (location.pos < code.length()) {
				if (peek() == '*' && next() == '/') {
					advance();
					advance();
					return true;
				}
				advance();
			}
			return false;
		}
		return true;
	}

	char Lexer::peek() const {
		if (location.pos >= code.length())
			return '\0';
		return code[location.pos];
	}

	char Lexer::next() const {
		if (location.pos + 1 >= code.length())
			return '\0';
		return code[location.pos + 1];
	}

	char Lexer::advance() {
		if (location.pos >= code.length())
			return '\0';

		char c = code[location.pos];
		if (c == '\n') {
			location.line++;
			location.column = 1;
		}
		else location.column++;

		location.pos++;
		return c;
	}

}
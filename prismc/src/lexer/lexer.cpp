
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

	std::expected<std::vector<Token>, uint16_t> Lexer::lex(const File& file) {
		code = file.get_code();
		location.filepath = &file.get_path();
		location.pos = 0;
		location.line = 1;
		location.column = 1;

		std::vector<Token> tokens;

		tokens.reserve(code.length() / 4);

		while (location.pos < code.length()) {
			SourceLocation comment_start = location;
			if (!skip()) {
				return std::unexpected(diag.log(err::SyntaxUnclosedComment, comment_start.as_string()));
			}

			if (location.pos >= code.length()) {
				break;
			}

			SourceLocation token_start = location;
			uint32_t start_pos = location.pos;

			char c = peek();

			if (c == '"') {
				auto str_res = lex_string();
				if (!str_res) {
					return std::unexpected(diag.log(err::SyntaxUnclosedStringLiteral, token_start.as_string()));
				}
				tokens.push_back(str_res.value());
				continue;
			}

			if (std::isdigit(static_cast<unsigned char>(c))) {
				tokens.push_back(lex_number());
				continue;
			}

			if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || static_cast<unsigned char>(c) >= 0x80) {
				tokens.push_back(lex_identifier_or_keyword());
				continue;
			}

			advance();
			TokenType type = TokenType::Invalid;

			switch (c) {
				case ':': type = TokenType::Colon; break;
				case ';': type = TokenType::Semicolon; break;
				case ',': type = TokenType::Comma; break;
				case '.': type = TokenType::Dot; break;
				case '(': type = TokenType::OpenParen; break;
				case ')': type = TokenType::CloseParen; break;
				case '{': type = TokenType::OpenBrace; break;
				case '}': type = TokenType::CloseBrace; break;
				case '[': type = TokenType::OpenBracket; break;
				case ']': type = TokenType::CloseBracket; break;
				case '*': type = TokenType::Asterisk; break;
				case '/': type = TokenType::Slash; break;
				case '%': type = TokenType::Percent; break;
				case '^': type = TokenType::Caret; break;
				case '~': type = TokenType::Tilde; break;

				case '=':
					if (peek() == '=') { advance(); type = TokenType::Equal; }
					else { type = TokenType::Assign; }
					break;
				case '!':
					if (peek() == '=') { advance(); type = TokenType::NotEqual; }
					else { type = TokenType::Exclamation; }
					break;
				case '<':
					if (peek() == '=') { advance(); type = TokenType::LessEqual; }
					else if (peek() == '<') { advance(); type = TokenType::LeftShift; }
					else { type = TokenType::LessThan; }
					break;
				case '>':
					if (peek() == '=') { advance(); type = TokenType::GreaterEqual; }
					else if (peek() == '>') { advance(); type = TokenType::RightShift; }
					else { type = TokenType::GreaterThan; }
					break;
				case '+':
					type = TokenType::Plus;
					break;
				case '-':
					type = TokenType::Minus;
					break;

				case '&':
					if (peek() == '&') { advance(); type = TokenType::AmpAmp; }
					else { type = TokenType::Amp; }
					break;
				case '|':
					if (peek() == '|') { advance(); type = TokenType::PipePipe; }
					else { type = TokenType::Pipe; }
					break;

				default:
					type = TokenType::Invalid;
					break;
			}

			uint8_t length = static_cast<uint8_t>(location.pos - start_pos);
			tokens.push_back(Token{ type, length, token_start });
		}

		tokens.push_back(Token{ TokenType::EoF, 0, location });

		return tokens;
	}

	Token Lexer::lex_identifier_or_keyword() {
		SourceLocation start_loc = location;
		uint32_t start_pos = location.pos;

		advance();

		while (location.pos < code.length()) {
			unsigned char c = static_cast<unsigned char>(peek());

			if (std::isalnum(c) || c == '_') {
				advance();
			}
			else if (c >= 0x80) {
				advance();
			}
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

	std::expected<Token, bool> Lexer::lex_string() {
		SourceLocation start_loc = location;

		advance();

		while (peek() != '"' && peek() != '\0') {
			advance();
		}

		if (peek() == '"') {
			advance();

			uint8_t length = static_cast<uint8_t>(location.pos - start_loc.pos);
			return Token{ TokenType::StringLiteral, length, start_loc };
		}

		return std::unexpected(false);
	}

	Token Lexer::lex_number() {
		SourceLocation start_loc = location;

		while (std::isdigit(static_cast<unsigned char>(peek()))) {
			advance();
		}

		TokenType type = TokenType::IntLiteral;

		if (peek() == '.' && std::isdigit(static_cast<unsigned char>(next()))) {
			type = TokenType::FloatLiteral;
			advance();

			while (std::isdigit(static_cast<unsigned char>(peek()))) {
				advance();
			}
		}

		uint8_t length = static_cast<uint8_t>(location.pos - start_loc.pos);
		return Token{ type, length, start_loc };
	}

	bool Lexer::skip() {
		uint32_t last_pos = location.pos;

		while (location.pos < code.length()) {
			skip_whitespace();
			skip_comment();

			if (!skip_multiline_comment()) return false;

			if (location.pos == last_pos) break;
			last_pos = location.pos;
		}
		return true;
	}

	void Lexer::skip_whitespace() {
		while (location.pos < code.length()) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				advance();
			}
			else break;
		}
	}

	void Lexer::skip_comment() {
		if (peek() == '/' && next() == '/') {
			while (peek() != '\n' && peek() != '\0') {
				advance();
			}
		}
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
		if (location.pos >= code.length()) return '\0';
		return code[location.pos];
	}

	char Lexer::next() const {
		if (location.pos + 1 >= code.length()) return '\0';
		return code[location.pos + 1];
	}

	char Lexer::advance() {
		if (location.pos >= code.length()) return '\0';

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
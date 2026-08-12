#include "include/lexer/lexer.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/logger/logger.hpp"

#include <sstream>
#include <iomanip>

const std::unordered_map<std::string, prism::TokenType> prism::Lexer::keywords = {
	{"entry", prism::TokenType::KEYWORD_ENTRY},
	{"return", prism::TokenType::KEYWORD_RETURN}
};

prism::Lexer::Lexer(std::string_view& code, const CompilerConfig& compilerconfig) {
	tokens.reserve(code.size() / 4);

	size_t i = 0;
	size_t length = code.size();

	while (i < length) {
		char curr = code[i];

		if (std::isspace(curr)) {
			i++;

			continue;
		}

		if (curr == '/' && i + 1 < length && code[i + 1] == '/') {
			while (i < length && code[i] != '\n' && code[i] != '\r') {
				i++;
			}
			continue;
		}

		if (curr == '{') {
			tokens.push_back({ "{", TokenType::LBRACE, std::monostate{} });
			i++;

			continue;
		}
		if (curr == '}') {
			tokens.push_back({ "}", TokenType::RBRACE, std::monostate{} });
			i++;

			continue;
		}
		if (curr == '(') {
			tokens.push_back({ "(", TokenType::LPAREN, std::monostate{} });
			i++;

			continue;
		}
		if (curr == ')') {
			tokens.push_back({ ")", TokenType::RPAREN, std::monostate{} });
			i++;

			continue;
		}
		if (curr == ';') {
			tokens.push_back({ ";", TokenType::SEMICOLON, std::monostate{} });
			i++;

			continue;
		}

		if (std::isalpha(curr) || curr == '_') {
			std::string identifier;

			while (i < length && (std::isalnum(code[i]) || code[i] == '_')) {
				identifier += code[i];
				i++;
			}

			auto it = keywords.find(identifier);
			if (it != keywords.end()) {
				tokens.push_back({ identifier, it->second, std::monostate{} });
			} else {
				tokens.push_back({ identifier, TokenType::IDENTIFIER, std::monostate{} });
			}

			continue;
		}

		if (std::isdigit(curr)) {
			std::string num_str;
			bool is_int = true;
			bool is_long_long = false;
			bool is_float = false;
			bool is_double = false;
			bool has_error = false;

			while (i < length) {
				char next = code[i];

				if (std::isdigit(next)) {
					num_str += next;
					i++;
				} else if (next == '.') {
					if (is_double) {
						has_error = true;
						num_str += next;
					}
					is_double = true;
					num_str += next;
					i++;
				} else if (next == 'f' && is_double) {
					is_double = false;
					is_float = true;
				} else if (next == 'L' && code[i + 1] == 'L' && is_int) {
					is_int = false;
					is_long_long = true;
				} else {
					break;
				}
			}

			if (has_error) {
				ErrorHandler errorhandler;
				errorhandler.log(ERROR_MULTIPLE_DOTS, compilerconfig, num_str);
				tokens.push_back({ num_str, TokenType::INVALID, std::monostate() });

				continue;
			}

			if (is_double) {
				double value = std::stod(num_str);
				tokens.push_back({ num_str, TokenType::LITERAL_DOUBLE, value });
			} else if (is_float) {
				float value = std::stof(num_str);
				tokens.push_back({ num_str, TokenType::LITERAL_FLOAT, value });
			} else if (is_long_long) {
				long long value = std::stoll(num_str);
				tokens.push_back({ num_str, TokenType::LITERAL_LL, value });
			} else {
				int value = std::stoi(num_str);
				tokens.push_back({ num_str, TokenType::LITERAL_INT, value });
			}

			continue;
		}

		std::string unknown_char(1, curr);
		tokens.push_back({ unknown_char, TokenType::UNKNOWN, std::monostate{} });
		i++;
	}

	tokens.push_back({ "EndOfFile", TokenType::EOFILE, std::monostate{} });

	for (const auto& token : tokens) {
		if (token.type == TokenType::INVALID) {
			ErrorHandler errorhandler;
			errorhandler.log(ERROR_INVALID_TOKEN, compilerconfig, token.lexeme);
		} else if (token.type == TokenType::UNKNOWN) {
			ErrorHandler errorhandler;
			errorhandler.log(ERROR_UNKNOWN_TOKEN, compilerconfig, token.lexeme);
		}
	}

	if (compilerconfig.dump_tokens) {
		size_t lexeme_len = 0;
		for (const auto& token : tokens) {
			if (token.lexeme.size() > lexeme_len) {
				lexeme_len = token.lexeme.size();
			}
		}

		std::ostringstream dump;
		for (size_t i = 0; i < tokens.size(); i++) {
			const auto& token = tokens[i];

			std::string translation;
			std::string value;
			std::string spacing = "               ";
			bool has_error = false;
			switch (token.type) {
				case TokenType::EOFILE: translation = "EOFILE"; break;

				case TokenType::KEYWORD_ENTRY: translation = "KEYWORD_ENTRY"; break;
				case TokenType::KEYWORD_RETURN: translation = "KEYWORD_RETURN"; break;

				case TokenType::IDENTIFIER:    translation = "IDENTIFIER"; break;

				case TokenType::LBRACE:        translation = "LBRACE"; break;
				case TokenType::RBRACE:        translation = "RBRACE"; break;

				case TokenType::LPAREN:        translation = "LPAREN"; break;
				case TokenType::RPAREN:        translation = "RPAREN"; break;

				case TokenType::SEMICOLON:     translation = "SEMICOLON"; break;

				case TokenType::LITERAL_INT:   translation = "LITERAL_INT"; break;
				case TokenType::LITERAL_LL:    translation = "LITERAL_LL"; break;
				case TokenType::LITERAL_FLOAT: translation = "LITERAL_FLOAT"; break;
				case TokenType::LITERAL_DOUBLE: translation = "LITERAL_DOUBLE"; break;

				case TokenType::INVALID:       translation = "INVALID"; has_error = true; break;
				case TokenType::UNKNOWN:       translation = "UNKNOWN"; has_error = true; break;

				default:                       translation = "???"; has_error = true; break;
			}

			if (has_error) dump << "[220, 53, 69]";
			else dump << "[30, 167, 69]";
			dump << "(" << i << "):\t\t [] [108, 255, 123]Lexeme:[] [78, 201, 176]"
				<< std::left << std::setw(lexeme_len) << token.lexeme
				<< "[] [237, 146, 36]|[] [147, 147, 249]Token type:[] [78, 201, 176]"
				<< std::left << std::setw(14) << translation
				<< "[] [237, 146, 36]|[] [255, 121, 198]Value:[] [78, 201, 176]";

			std::visit([&value](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::monostate>) {
					value = "None";
				} else value = std::to_string(arg);
			}, token.value);

			dump << value << "[]\n";
		}
		ErrorHandler errorhandler;
		errorhandler.log(INFO_TOKENS_DUMP, compilerconfig, compilerconfig.filepath, dump.str());
	}
}
#pragma once

#include <string>
#include <variant>

namespace prism {

	enum TokenType {
		KEYWORD_ENTRY,
		KEYWORD_IF,
		KEYWORD_ALT,

		LBRACE,
		RBRACE,

		SEMICOLON,

		LITERAL_INT,

		UNKNOWN,
	};

	struct Token {
		TokenType type;
		std::string lexeme;
		std::variant<std::monostate, int32_t> value;
	};

}
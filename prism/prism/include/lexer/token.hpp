#pragma once

#include <string>
#include <variant>

namespace prism {

	enum TokenType {
		KEYWORD_ENTRY,
		KEYWORD_RETURN,

		IDENTIFIER,

		LBRACE,
		RBRACE,

		SEMICOLON,

		LITERAL_INT,
		LITERAL_FLOAT,

		INVALID,
		UNKNOWN,
	};

	struct Token {
		TokenType type;
		std::string lexeme;
		std::variant<std::monostate, int, double> value;
	};

}
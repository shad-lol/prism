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
		LITERAL_LL,
		LITERAL_FLOAT,
		LITERAL_DOUBLE,

		INVALID,
		UNKNOWN,
	};

	struct Token {
		std::string lexeme;
		TokenType type;
		std::variant<std::monostate, int, long long, float, double> value;
	};

}
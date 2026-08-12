#pragma once

#include <string>
#include <variant>

namespace prism {

	enum TokenType {
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
#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>

#include "token.hpp"
#include "arg_reader.hpp"
#include "compiler_config.hpp"

namespace prism {

	class Lexer {
	private:
		std::vector<Token> tokens;
		static const std::unordered_map<std::string, TokenType> keywords;

	public:
		Lexer(std::string_view& code, const CompilerConfig& compilerconfig);
		std::vector<Token> get() { return tokens; }
	};

}
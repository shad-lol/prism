#pragma once

#include "token.hpp"
#include "include/arg_reader/arg_reader.hpp"
#include "include/compiler_config/compiler_config.hpp"

#include <vector>
#include <string_view>
#include <unordered_map>

namespace prism {

	class Lexer {
	private:
		std::vector<Token> tokens;
		static const std::unordered_map<std::string, TokenType> keywords;

	public:
		Lexer(const std::string_view& code, const CompilerConfig& compilerconfig);
		std::vector<Token> get() { return tokens; }
	};

}
#pragma once

#include "include/lexer/token.hpp"
#include "include/parser/node.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/lexer/compiler_config.hpp"

#include <vector>

namespace prism {

	class Parser {
	private:
		std::vector<Token> tokens;
		Node root{ Token{"root", TokenType::ROOT, std::monostate{}} };
		size_t pos = 0;
		
	public:
		Parser(std::vector<Token> tokens, CompilerConfig compilerconfig);

		std::unique_ptr<Node> parseEntry(CompilerConfig compilerconfig);

		std::unique_ptr<Node> parseFunctionName(CompilerConfig compilerconfig);
		std::unique_ptr<Node> parseFunctionArguements(CompilerConfig compilerconfig);
		std::unique_ptr<Node> parseFunctionBody(CompilerConfig compilerconfig);

		std::unique_ptr<Node> parseReturnValue();
		
		bool match(TokenType expected);
		Token pop() { return tokens[pos++]; }
		Token consume(TokenType expected, err error_code, CompilerConfig compilerconfig);

		void advance() { pos++; }

		int max_depth(const Node& node, int depth = 0);
		void print_tree(const Node& node, bool utf8, int max_depth, CompilerConfig compilerconfig, std::string prefix = "", bool isLast = false, int depth = 0);

		const Node& get();
	};

}
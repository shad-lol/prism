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
		CompilerConfig compilerconfig;
		
	public:
		Parser(std::vector<Token> tokens, CompilerConfig compilerconfig);

		std::unique_ptr<Node> parseEntry();

		std::unique_ptr<Node> parseFunctionName();
		std::unique_ptr<Node> parseFunctionArguements();
		std::unique_ptr<Node> parseFunctionBody();

		std::unique_ptr<Node> parseReturnValue();
		
		bool match(TokenType expected);
		Token pop() { return tokens[pos++]; }
		Token consume(TokenType expected, err error_code);

		void advance() { pos++; }

		int max_depth(const Node& node, int depth = 0);
		std::string print_tree(const Node& node, std::stringstream& ss, bool utf8, int max_depth, std::string prefix = "", bool isLast = false, int depth = 0);

		const Node& get();
	};

}
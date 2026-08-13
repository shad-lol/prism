/*
 *  Copyright 2026 The Prism Authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include "include/lexer/token.hpp"
#include "include/parser/node.hpp"
#include "include/error_handler/error_handler.hpp"
#include "include/compiler_config/compiler_config.hpp"

#include <vector>

namespace prism {

	class Parser {
	private:
		std::vector<Token> tokens;
		Node root{ Token{"root", TokenType::ROOT, std::monostate{}} };
		size_t pos = 0;
		CompilerConfig compilerconfig;
		
	public:
		Parser(std::vector<Token> tokens, const CompilerConfig& compilerconfig);

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
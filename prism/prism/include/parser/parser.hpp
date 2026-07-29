#pragma once

#include <vector>

#include "include/lexer/token.hpp"
#include "include/parser/node.hpp"

namespace prism {

	class Parser {
	private:
		std::vector<Node&> nodes;
		
	public:
		void adopt(std::vector<Node&>& children);
	};

}
#pragma once

#include <vector>
#include <memory>

#include "include/lexer/token.hpp"

namespace prism {

	struct Node {
		Token value;
		std::vector<std::unique_ptr<Node>> children;

		Node(Token val) : value(val) {}
	};

}
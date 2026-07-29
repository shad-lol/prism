#pragma once

#include <vector>

#include "include/lexer/token.hpp"

namespace prism {

	struct Node {
		Token handle;
		std::vector<Node&> children;
	};

}
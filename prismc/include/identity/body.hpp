
/*   Copyright 2026 Prism Compiler Authors
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once

#include <variant>

namespace prismc {

	struct BinaryOpNode;
	struct VariableNode;
	struct CallNode;

	struct Expression {
		std::variant<
			std::monostate,

			int8_t, int32_t,
			int16_t, int64_t,

			uint8_t, uint32_t,
			uint16_t, uint64_t,

			float, double,

			std::string,
			bool,

			std::shared_ptr<BinaryOpNode>,
			std::shared_ptr<VariableNode>,
			std::shared_ptr<CallNode>
		> value;
	};

	struct Argument {
		std::string label;
		Expression value;
	};

	struct BinaryOpNode {
		Expression left;
		std::string op;
		Expression right;
	};

	struct VariableNode {
		std::string identifier;
	};

	struct CallNode {
		std::string callee;
		std::vector<Argument> arguments;
	};

	struct ReturnNode {
		Expression expression;
	};

	struct VarDeclNode {
		std::string identifier;
		Expression initializer;
		bool is_mutable = false;
	};

	struct Body;
	struct Statement {
		std::variant<
			std::vector<VarDeclNode>,
			ReturnNode,
			CallNode,
			std::shared_ptr<Body>
		> statement;
	};

	struct Body {
		std::vector<Statement> statements;
	};

}
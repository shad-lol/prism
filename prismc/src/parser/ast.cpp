
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

#include <global.hpp>

#include <parser/ast.hpp>

#include <expected>

namespace prismc {

	bool AST::exists(const Signature& signature) {
		for (auto& func : funcs)
			if (func.signature == signature)
				return true;

		return false;
	}

	bool AST::exists(const std::string& type_name) {
		for (auto& struct_ : structs)
			if (struct_.identifier == type_name)
				return true;

		return false;
	}

	Func& AST::find(const Signature& signature) {
		for (auto& func : funcs)
			if (func.signature == signature)
				return func;
		return funcs[0];
	}

	uint32_t AST::find(std::string_view type_name) {
		for (size_t i = 0; i < structs.size(); i++)
			if (structs[i].identifier == type_name)
				return i;
		return UINT32_MAX;
	}

}
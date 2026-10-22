
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

#include <global.hpp>

#include <identity/definitions.hpp>

#include <unordered_set>
#include <expected>

namespace prismc {

	inline Struct Void = { "Void" };

	inline Struct Int8 = { "Int8" };
	inline Struct Int16 = { "Int16" };
	inline Struct Int32 = { "Int32" };
	inline Struct Int64 = { "Int64" };

	inline Struct UInt8 = { "UInt8" };
	inline Struct UInt16 = { "UInt16" };
	inline Struct UInt32 = { "UInt32" };
	inline Struct UInt64 = { "UInt64" };

	inline Struct Float32 = { "Float32" };
	inline Struct Float64 = { "Float64" };

	struct PRISMC_API AST {
		const std::filesystem::path* path;

		std::vector<Func> funcs;
		std::vector<Struct> structs = {
			Void,
			Int8, Int16, Int32, Int64,
			UInt8, UInt16, UInt32, UInt64,
			Float32, Float64
		};

		std::unordered_map<std::string, uint8_t> infix_precedences = {
			{"||", 1}, {"&&", 2},
			{"==", 3}, {"!=", 3},
			{"<",  4}, {"<=", 4}, {">",  4}, {">=", 4},
			{"+",  5}, {"-",  5},
			{"*",  6}, {"/",  6}, {"%",  6}
		};

		std::unordered_set<std::string> prefix_operators = {
			"-", "!", "√"
		};

		bool exists(const Signature& signature);
		bool exists(const std::string& type_name);
		Func& find(const Signature& signature);
		uint32_t find(std::string_view type_name);
	};

}

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

#include <bitset>
#include <optional>

namespace prismc {

	struct Parameter {
		std::string label;
		uint32_t typeID;

		bool operator==(const Parameter& other) const {
			return typeID == other.typeID;
		}
	};

	enum Modifiers {

		Entry = 0

	};

	struct Signature {
		std::string identifier;
		std::bitset<1> modifiers;
		std::vector<Parameter> parameters;
		uint32_t return_typeID;

		bool operator==(const Signature& other) const {
			return identifier == other.identifier &&
				   modifiers == other.modifiers &&
				   parameters == other.parameters;
		}
	};

}
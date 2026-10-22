
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

#include "signature.hpp"
#include "body.hpp"

namespace prismc {

	struct Func {
		Signature signature;
		Body body;
	};

	enum Publicity : uint8_t {
		Private = 0,
		Public = 1
	};

	struct Field {
		Publicity publicity;
		std::string identifier;
		uint32_t typeID;
	};

	struct Struct {
		std::string identifier;
		std::vector<Field> layout;
		std::vector<Func> methods;
	};

}
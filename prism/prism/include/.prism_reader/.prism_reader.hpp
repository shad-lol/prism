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

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "include/compiler_config/compiler_config.hpp"

namespace prism {

	class PrismReader {
	private:
		std::ifstream ifstream_code;
		std::ostringstream ostringstream_code;
		std::string string_code;

	public:
		PrismReader(const CompilerConfig& compilerconfig);
		~PrismReader();

		std::string_view get() { return string_code; }

		bool is_open() { return ifstream_code.is_open(); }
	};

}
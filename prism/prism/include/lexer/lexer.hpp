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

#include "token.hpp"
#include "include/arg_reader/arg_reader.hpp"
#include "include/compiler_config/compiler_config.hpp"

#include <vector>
#include <string_view>
#include <unordered_map>

namespace prism {

	class Lexer {
	private:
		std::vector<Token> tokens;
		static const std::unordered_map<std::string, TokenType> keywords;

	public:
		Lexer(const std::string_view& code, const CompilerConfig& compilerconfig);
		std::vector<Token> get() { return tokens; }
	};

}
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

#include "include/compiler_config/compiler_config.hpp"
#include "include/polariton_ir/pir_instruction.hpp"
#include "include/polariton_ir/pir_unit.hpp"

#include <string>
#include <vector>
#include <variant>

namespace prism {

	class CodeGen {
	private:
		std::vector<PIRUnit> PIRStream;
		std::string Ccode;

	public:
		CodeGen(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig);

		void C(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig);
		void LLVM(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig);

		std::string get() { return Ccode; }
	};

}
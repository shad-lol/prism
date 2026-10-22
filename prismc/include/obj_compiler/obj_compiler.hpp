
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

#ifdef __INTELLISENSE__
#undef _MSC_VER
#define _MSC_VER 1951 
#undef __clang__
#endif

#include "llvm/Target/TargetMachine.h"
#include <llvm/Object/ObjectFile.h>

#include <expected>

namespace prismc {

	class PRISMC_API ObjCompiler {

	public:
		uint16_t init();
		std::expected<std::reference_wrapper<const llvm::DataLayout>, uint16_t> data_layout() const;
		std::expected<std::reference_wrapper<const llvm::Triple>, uint16_t> triple() const;
		uint16_t compile(llvm::Module& ir, std::filesystem::path path);

	private:
		std::unique_ptr<llvm::TargetMachine> target_machine;
		llvm::DataLayout _data_layout;
		bool initialized = false;

	};

}

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

#include <obj_compiler/obj_compiler.hpp>

#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"

#include <filesystem>

namespace prismc {

	uint16_t ObjCompiler::init() {
		if (initialized) return err::Success;

		llvm::InitializeAllTargetInfos();
		llvm::InitializeAllTargets();
		llvm::InitializeAllTargetMCs();
		llvm::InitializeAllAsmParsers();
		llvm::InitializeAllAsmPrinters();

		llvm::Triple triple(llvm::sys::getDefaultTargetTriple());
		std::string error;

		const llvm::Target* target = llvm::TargetRegistry::lookupTarget(triple, error);
		if (!target)
			return diag.log(err::ErrTargetMachineInitError, error);

		llvm::TargetOptions options;

		target_machine = std::unique_ptr<llvm::TargetMachine>(
			target->createTargetMachine(
				triple,
				"generic",
				"",
				options,
				std::nullopt,
				std::nullopt,
				llvm::CodeGenOptLevel::None
			)
		);

		_data_layout = target_machine->createDataLayout();
		initialized = true;

		return err::Success;
	}

	std::expected<std::reference_wrapper<const llvm::DataLayout>, uint16_t> ObjCompiler::data_layout() const {
		if (!initialized)
			return std::unexpected(diag.log(err::ErrNotInitialized));

		return std::cref(_data_layout);
	}

	std::expected<std::reference_wrapper<const llvm::Triple>, uint16_t> ObjCompiler::triple() const {
		if (!initialized)
			return std::unexpected(diag.log(err::ErrNotInitialized));

		return std::cref(target_machine->getTargetTriple());
	}

	uint16_t ObjCompiler::compile(llvm::Module& module, std::filesystem::path path) {
		if (!initialized)
			return diag.log(err::ErrNotInitialized);

		if (path.extension() != ".obj")
			path += ".obj";

		std::error_code ec;
		llvm::raw_fd_ostream dest(path.string(), ec, llvm::sys::fs::OF_None);

		if (ec)
			return diag.log(err::ErrOutputFileError);

		llvm::legacy::PassManager pass;
		if (target_machine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile))
			return diag.log(err::ErrEmitFileUnsupported);

		pass.run(module);
		dest.flush();

		return err::Success;
	}

}
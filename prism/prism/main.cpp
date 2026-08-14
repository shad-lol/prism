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

#ifdef _WIN32
	#include <Windows.h>
	#undef ERROR
	#undef ERROR_FILE_NOT_FOUND
	#undef ERROR_INVALID_TOKEN
	#undef VOID
#endif

#include "include/arg_reader/arg_reader.hpp"
#include "include/.prism_reader/.prism_reader.hpp"
#include "include/lexer/lexer.hpp"
#include "include/parser/parser.hpp"
#include "include/polariton_ir/pir_builder.hpp"
#include "include/codegen/codegen.hpp"
#include "include/backend/backend.hpp"

int main(int argc, char* argv[]) {

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	prism::CompilerConfig compilerconfig;
	prism::ArgReader argreader(argc, argv, compilerconfig);

	for (size_t i = 0; i < argreader.filecount(); i++) {
		compilerconfig.filepath = argreader.file(i);
		compilerconfig.filedir = compilerconfig.filepath.parent_path();

		prism::PrismReader prismreader(compilerconfig);
		prism::Lexer lexer(prismreader.get(), compilerconfig);

		prism::Parser parser(lexer.get(), compilerconfig);

		prism::PIRBuilder pirbuilder(parser.get(), compilerconfig);

		prism::CodeGen codegen(pirbuilder.get(), compilerconfig);

		prism::Backend backend(codegen.get(), compilerconfig);
	}

	return 0;
}
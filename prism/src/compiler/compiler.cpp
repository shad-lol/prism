
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

#include <compiler/compiler.hpp>
#include <chrono>

namespace prism {

	Compiler::Compiler() {
		prismc::register_codes();
		verify(objc.init());
	}

	void Compiler::compile(std::vector<std::filesystem::path> file_paths) {
		const auto start = std::chrono::high_resolution_clock::now();

		const auto data_layout = objc.data_layout();
		verify(data_layout);

		const auto triple = objc.triple();
		verify(triple);

		std::string entry_point;

		std::vector<std::filesystem::path> obj_files;
		obj_files.reserve(file_paths.size());

		for (const auto& path : file_paths) {
			prismc::File file;
			verify(file.load(path));

			prismc::Lexer lexer;
			prismc::Parser parser;

			auto tokens = lexer.tokenize(file);
			verify(tokens);

			auto ast = parser.parse(*tokens, file.code);
			verify(ast);
			
			prismc::IRGenerator irgen;
			auto ir = irgen.compile(*ast, *data_layout, *triple);
			verify(ir);

			(*ir)->print(llvm::outs(), nullptr);

			verify(objc.compile(**ir, file.path));
			obj_files.push_back(file.path);
		}

		verify(prismc::link(obj_files, file_paths.front()));

		const auto end = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<double, std::milli> total_duration = end - start;

		std::cout << "\n-------------------------------------------\n";
		std::cout << "Compilation time: " << total_duration.count() << " ms\n\n";

		file_paths.front().replace_extension(".exe");
		std::cout << "Program returned: " << std::system(std::filesystem::absolute(file_paths.front()).string().c_str()) << '\n';
	}

}
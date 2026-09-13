
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

#include <prismc>
	
int main() {
	prismc::register_codes();

	prismc::File file;
	std::filesystem::path out = "C:/Users/user/Desktop/material/test/out/test/";

	prismc::Lexer lexer;
//	prismc::Parser parser;
//	prismc::Backend backend;

	auto res = file.load("C:/Users/user/Desktop/material/test/src/test.prism");
	if (!res) return res.error();

//	auto token_stream = lexer.lex(file);
//	if (!token_stream) return token_stream.error();

//	auto ast = parser.parse(token_stream);
//	if (!ast) return ast.error();

//	auto res = backend.compile(ast, out);
//	if (!res) return res.error();

	return 0;
}
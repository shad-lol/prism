
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

#include <parser/ast.hpp>
#include <lexer/token.hpp>

namespace prismc {

	class PRISMC_API Parser {

	public:
		std::expected<AST, uint16_t> parse(const std::vector<Token>& tokens, const std::string_view& code);

	private:
		AST ast;
		std::vector<Token> tokens;
		uint32_t pos;
		std::string_view code;

		uint16_t parse_declarations();
		uint16_t parse_function_declaration();
		uint16_t parse_struct_declaration();

		uint16_t parse_bodies();
		std::expected<Body, uint16_t> parse_body();
		std::expected<Statement, uint16_t> parse_statement();

		std::expected<Expression, uint16_t> parse_expression(uint8_t precedence = 0);
		std::expected<Expression, uint16_t> parse_prefix();

		[[nodiscard]] constexpr Func& last_func() noexcept { return ast.funcs.back(); }
		[[nodiscard]] constexpr Struct& last_struct() noexcept { return ast.structs.back(); }

		std::expected<uint8_t, uint16_t> get_current_op_precedence();
		[[nodiscard]] Token curr() const { return tokens[pos]; }

	};

}
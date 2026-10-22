
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

#include <parser/parser.hpp>

namespace prismc {

	std::expected<AST, uint16_t> Parser::parse(const std::vector<Token>& tokens, const std::string_view& code) {
		this->tokens = tokens;
		this->code = code;
		ast.path = tokens[0].location.path;

		pos = 0;
		uint16_t res = parse_declarations();
		if (res != err::Success) return std::unexpected(res);

		pos = 0;
		res = parse_bodies();
		if (res != err::Success) return std::unexpected(res);

		return ast;
	}

	uint16_t Parser::parse_declarations() {
		while (!curr().is(TokenType::EoF)) {
			std::initializer_list declaration_types = { TokenType::KwEntry, TokenType::KwFunc, TokenType::KwStruct };

			if (curr().is_one_of(declaration_types)) {
				if (curr().is(TokenType::KwStruct)) {
					ast.structs.push_back(Struct());
					uint16_t res = parse_struct_declaration();
					if (res != err::Success) return res;
				}
				else {
					ast.funcs.push_back(Func());
					uint16_t res = parse_function_declaration();
					if (res != err::Success) return res;
				}
			}

			pos++;
		}

		return err::Success;
	}

	uint16_t Parser::parse_function_declaration() {
		Func& func = last_func();

		if (curr().is(TokenType::KwEntry)) {
			func.signature.modifiers[Entry] = true;
			pos++;
		}

		if (!curr().is(TokenType::KwFunc))
			return diag.log(err::SyntaxExpectedFuncKeyword, curr().location.as_string());
		pos++;

		if (!curr().is(TokenType::Identifier))
			return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

		Signature signature = func.signature;
		signature.identifier = curr().lexeme(code);

		if (ast.exists(signature))
			return diag.log(err::SyntaxRedifinition, curr().location.as_string(), "function", signature.identifier);

		func.signature.identifier = signature.identifier;
		pos++;

		if (!curr().is(TokenType::OpenParen))
			return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "(");
		pos++;

		std::vector<Parameter> parameters;
		while (!curr().is(TokenType::CloseParen)) {
			Parameter parameter;

			if (!curr().is(TokenType::Identifier))
				return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

			parameter.label = curr().lexeme(code);
			pos++;

			if (!curr().is(TokenType::Colon))
				return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), ":");
			pos++;

			if (!curr().is(TokenType::Identifier))
				return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

			parameter.typeID = ast.find(curr().lexeme(code));
			if (parameter.typeID == UINT32_MAX)
				return diag.log(err::SyntaxUndeclaredIdentifier, curr().location.as_string(), curr().lexeme(code));

			parameters.push_back(parameter);
			pos++;

			if (!curr().is(TokenType::CloseParen) && !curr().is(TokenType::Comma))
				return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), ",");

			if (curr().is(TokenType::CloseParen))
				break;
			pos++;
		}

		func.signature.parameters = parameters;
		pos++;

		if (curr().lexeme(code) == "->") {
			if (func.signature.modifiers[Entry])
				return diag.log(err::SyntaxCustomRetTypeForEntry, curr().location.as_string(), tokens[pos + 1].lexeme(code), func.signature.identifier);

			pos++;

			if (!curr().is(TokenType::Identifier))
				return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

			func.signature.return_typeID = ast.find(curr().lexeme(code));
			if (func.signature.return_typeID == UINT32_MAX)
				return diag.log(err::SyntaxUndeclaredIdentifier, curr().location.as_string(), curr().lexeme(code));

			pos++;
		}
		else {
			if (func.signature.modifiers[Entry])
				func.signature.return_typeID = 3;
			else
				func.signature.return_typeID = 0;
		}

		return err::Success;
	}

	uint16_t Parser::parse_struct_declaration() {
		Struct& struct_ = last_struct();

		pos++;

		if (!curr().is(TokenType::Identifier))
			return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

		if (ast.exists(curr().lexeme(code)))
			return diag.log(err::SyntaxRedifinition, curr().location.as_string(), "struct", struct_.identifier);
		pos++;

		struct_.identifier = curr().lexeme(code);

		if (!curr().is(TokenType::OpenBrace))
			return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "{");
		pos++;

		while (!curr().is(TokenType::CloseBrace) && !curr().is(TokenType::EoF)) {
			Publicity publicity = (curr().is(TokenType::KwPublic)) ? Public : Private;
			if (curr().is(TokenType::KwPublic) || curr().is(TokenType::KwPrivate))
				pos++;

			if (!curr().is(TokenType::Identifier))
				return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

			std::string field_name = curr().lexeme(code);
			pos++;

			if (!curr().is(TokenType::Colon))
				return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), ":");
			pos++;

			if (!curr().is(TokenType::Identifier))
				return diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string());

			uint32_t field_typeID = ast.find(curr().lexeme(code));
			if (field_typeID == UINT32_MAX)
				return diag.log(err::SyntaxUndeclaredIdentifier, curr().location.as_string(), curr().lexeme(code));

			struct_.layout.push_back(Field{ publicity, std::move(field_name), field_typeID });
			pos++;

			if (!curr().is(TokenType::NewLine) && !curr().is(TokenType::Comma))
				return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), ",");
			pos++;

			if (curr().is(TokenType::CloseBrace))
				break;
			pos++;
		}

		if (!curr().is(TokenType::CloseBrace))
			return diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "}");
		pos++;

		return err::Success;
	}

	uint16_t Parser::parse_bodies() {
		size_t func_idx = 0;

		while (!curr().is(TokenType::EoF)) {
			if (curr().is(TokenType::KwFunc)) {
				pos++;
				if (curr().is(TokenType::EoF))
					return diag.log(err::SyntaxIdentifierNotGiven, curr().location.as_string(), "function");

				Func& func = ast.funcs[func_idx++];

				while (!curr().is(TokenType::OpenBrace) && !curr().is(TokenType::EoF))
					pos++;
				
				auto res = parse_body();
				if (!res) return res.error();

				func.body = std::move(*res);
			}
			else pos++;
		}

		return err::Success;
	}

	std::expected<Body, uint16_t> Parser::parse_body() {
		if (!curr().is(TokenType::OpenBrace))
			return std::unexpected(diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "{"));
		pos++;

		Body body;

		while (!curr().is(TokenType::CloseBrace) && !curr().is(TokenType::EoF)) {
			auto res = parse_statement();
			if (!res) return std::unexpected(res.error());

			body.statements.push_back(std::move(*res));
		}
		
		if (!curr().is(TokenType::CloseBrace))
			return std::unexpected(diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "}"));
		pos++;

		return body;
	}

	std::expected<Statement, uint16_t> Parser::parse_statement() {
		switch (curr().type) {
			case TokenType::KwReturn: {
				pos++;

				if (curr().is(TokenType::NewLine)) {
					pos++;
					return Statement{ ReturnNode{ Expression{} } };
				}

				auto res = parse_expression();
				if (!res) return std::unexpected(res.error());

				if (!curr().is(TokenType::NewLine))
					return std::unexpected(diag.log(err::SyntaxExpectedEndOfLine, curr().location.as_string()));
				pos++;

				return Statement{ ReturnNode{ std::move(*res) } };
			}

			case TokenType::KwLet:
			case TokenType::KwVar: {
				bool is_mutable = (curr().type == TokenType::KwLet) ? false : true;

				pos++;

				std::vector<VarDeclNode> variables;
				while (!curr().is(TokenType::NewLine)) {
					if (!curr().is(TokenType::Identifier))
						return std::unexpected(diag.log(err::SyntaxExpectedIdentifier, curr().location.as_string()));

					std::string identifier = curr().lexeme(code);
					pos++;

					if (curr().lexeme(code) != "=")
						return std::unexpected(diag.log(err::SyntaxExpectedToken, curr().location.as_string(), "="));
					pos++;

					auto res = parse_expression();
					if (!res) return std::unexpected(res.error());

					variables.push_back(VarDeclNode{
						std::move(identifier),
						std::move(*res),
						is_mutable
					});

					if(!curr().is(TokenType::NewLine) && !curr().is(TokenType::Comma))
						return std::unexpected(diag.log(err::SyntaxExpectedToken, curr().location.as_string(), ","));

					if (curr().is(TokenType::Comma)) {
						pos++;
					}
				}

				if (!curr().is(TokenType::NewLine))
					return std::unexpected(diag.log(err::SyntaxExpectedEndOfLine, curr().location.as_string()));
				pos++;

				return Statement{ variables };
			}

			case TokenType::Identifier: {
				auto expr_res = parse_expression();
				if (!expr_res) return std::unexpected(expr_res.error());

				if (!curr().is(TokenType::NewLine))
					return std::unexpected(diag.log(err::SyntaxExpectedEndOfLine, curr().location.as_string()));
				pos++;

				if (std::holds_alternative<std::shared_ptr<CallNode>>(expr_res->value)) {
					auto call_ptr = std::get<std::shared_ptr<CallNode>>(expr_res->value);
					return Statement{ *call_ptr };
				}

				return std::unexpected(diag.log(err::SyntaxExpectedStatement, curr().location.as_string()));
			}

			default:
				return std::unexpected(diag.log(err::SyntaxExpectedStatement, curr().location.as_string()));
		}
	}

	std::expected<Expression, uint16_t> Parser::parse_expression(uint8_t precedence) {
		auto left_res = parse_prefix();
		if (!left_res) return std::unexpected(left_res.error());

		Expression left = std::move(*left_res);

		auto res = get_current_op_precedence();
		if (!res) return std::unexpected(res.error());

		while (precedence < *res && !curr().is(TokenType::EoF)) {
			std::string op = curr().lexeme(code);
			uint8_t current_precedence = *res;
			pos++;

			auto right_res = parse_expression(current_precedence);
			if (!right_res) return std::unexpected(right_res.error());

			auto binary_node = std::make_shared<BinaryOpNode>(BinaryOpNode{
				std::move(left),
				op,
				std::move(*right_res)
			});

			left = Expression{ std::move(binary_node) };

			res = get_current_op_precedence();
			if (!res) return std::unexpected(res.error());
		}

		return left;
	}

	std::expected<Expression, uint16_t> Parser::parse_prefix() {
		switch (curr().type) {

			case TokenType::IntLiteral: {
				std::string lexeme;
				lexeme = curr().lexeme(code);

				pos++;
				return Expression{ static_cast<int32_t>(std::stoi(lexeme)) };
			}

			case TokenType::FloatLiteral: {
				std::string lexeme;
				lexeme = curr().lexeme(code);

				pos++;
				return Expression{ std::stof(lexeme) };
			}

			case TokenType::StringLiteral: {
				std::string lexeme;
				lexeme = curr().lexeme(code);

				pos++;
				return Expression{ lexeme };
			}

			case TokenType::BoolLiteral: {
				std::string lexeme;
				lexeme = curr().lexeme(code);

				pos++;
				return Expression{ lexeme == "true" };
			}

			case TokenType::OpenParen: {
				pos++;
				auto res = parse_expression(0);
				if (!res) return res;

				if (!curr().is(TokenType::CloseParen)) {
					return std::unexpected(diag.log(err::SyntaxExpectedToken, ")", curr().location.as_string()));
				}
				pos++;
				return res;
			}

			case TokenType::Operator: {
				std::string op;
				op = curr().lexeme(code);

				if (ast.prefix_operators.contains(op)) {
					pos++;

					auto res = parse_expression(10);
					if (!res) return res;

					auto unary_node = std::make_shared<BinaryOpNode>(BinaryOpNode{
						Expression{},
						op,
						std::move(*res)
					});

					return Expression{ std::move(unary_node) };
				}
				return std::unexpected(diag.log(err::SyntaxExpectedExpression, curr().location.as_string()));
			}

			case TokenType::Identifier: {
				std::string identifier = curr().lexeme(code);
				pos++;

				if (curr().is(TokenType::OpenParen)) {
					pos++;

					std::vector<Argument> arguments;

					if (!curr().is(TokenType::CloseParen)) {
						while (true) {
							std::string arg_label;

							if (curr().is(TokenType::Identifier) && (pos + 1 < tokens.size() && tokens[pos + 1].is(TokenType::Colon))) {
								arg_label = curr().lexeme(code);
								pos += 2;
							}

							auto arg_val_res = parse_expression(0);
							if (!arg_val_res) return std::unexpected(arg_val_res.error());

							arguments.push_back(Argument{ .label = arg_label, .value = std::move(*arg_val_res) });

							if (curr().is(TokenType::Comma)) {
								pos++;
								continue;
							}
							break;
						}
					}

					if (!curr().is(TokenType::CloseParen)) {
						return std::unexpected(diag.log(err::SyntaxExpectedToken, ")", curr().location.as_string()));
					}
					pos++;

					return Expression{ std::make_shared<CallNode>(CallNode{ std::move(identifier), std::move(arguments) }) };
				}

				auto variable_node = std::make_shared<VariableNode>(VariableNode{
					std::move(identifier)
				});

				return Expression{ std::move(variable_node) };
			}

			default:
				return std::unexpected(diag.log(err::SyntaxExpectedExpression, curr().location.as_string()));
		}
	}

	std::expected<uint8_t, uint16_t> Parser::get_current_op_precedence() {
		if (!curr().is(TokenType::Operator)) return 0;

		std::string op;
		op = curr().lexeme(code);

		if (auto it = ast.infix_precedences.find(op); it != ast.infix_precedences.end())
			return it->second;

		return 0;
	}

}
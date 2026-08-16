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

#include "include/polariton_ir/pir_builder.hpp" 

#include "include/error_handler/error_handler.hpp"

#include <iomanip>

prism::PIRBuilder::PIRBuilder(const Node& root, const CompilerConfig& compilerconfig) {
	generatePIR(root, compilerconfig);

	if (compilerconfig.dump_pir) {
		std::string dump;
		for (size_t i = 0; i < PIRStream.size(); i++) {
			std::string value = PIRStream[i].get_value_str();
            dump += "[30, 167, 69](" + std::to_string(i) + "):\t\t [] [93, 101, 156]Instruction: [][78, 201, 176]" + value + "[]\n";
        }

        ErrorHandler errorhandler;
        errorhandler.log(INFO_PIR_DUMP, compilerconfig, compilerconfig.filepath, dump);
    }
}

void prism::PIRBuilder::generatePIR(const Node& node, const CompilerConfig& compilerconfig) {
	switch (node.value.type) {
		case TokenType::FUNC:
			PIRStream.push_back(PIRInstruction::FUNC_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRInstruction::FUNC_END);
			break;

		case TokenType::KEYWORD_ENTRY:
			PIRStream.push_back(PIRType::ENTRY);
			break;

		case TokenType::FUNC_ARGUEMENTS:
			PIRStream.push_back(PIRInstruction::ARG_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRInstruction::ARG_END);
			break;

		case TokenType::FUNC_BODY:
			PIRStream.push_back(PIRInstruction::BODY_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRInstruction::BODY_END);
			break;

		case TokenType::KEYWORD_RETURN:
			PIRStream.push_back(PIRInstruction::RET);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			break;

		case TokenType::IDENTIFIER:
			PIRStream.push_back(PIRType::IDENTIFIER);
			PIRStream.push_back(node.value.lexeme);
			break;

		case TokenType::LITERAL_INT:
			PIRStream.push_back(PIRType::INT);
			PIRStream.push_back(std::get<int>(node.value.value));
			break;

		case TokenType::LITERAL_LL:
			PIRStream.push_back(PIRType::LONG_LONG);
			PIRStream.push_back(std::get<long long>(node.value.value));
			break;

		case TokenType::LITERAL_FLOAT:
			PIRStream.push_back(PIRType::FLOAT);
			PIRStream.push_back(std::get<float>(node.value.value));
			break;

		case TokenType::LITERAL_DOUBLE:
			PIRStream.push_back(PIRType::DOUBLE);
			PIRStream.push_back(std::get<double>(node.value.value));
			break;

		default:
			for (const auto& child : node.children) {
				generatePIR(*child, compilerconfig);
			}
			break;
	}
}
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
		std::ostringstream dump;
		for (size_t i = 0; i < PIRStream.size(); i++) {
			const auto& item = PIRStream[i];

			std::string raw_value = "";
			bool is_instruction = false;

			std::visit([&raw_value, &is_instruction](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, prism::PIRinstruction>) is_instruction = true;
                else if constexpr (std::is_same_v<T, std::string>) raw_value = arg;
                else raw_value = std::to_string(arg);
			}, item);

			if (is_instruction) {
				prism::PIRinstruction inst = std::get<prism::PIRinstruction>(item);
                switch (inst) {
					case prism::PIRinstruction::FUNC_START:  raw_value = "FUNC_START"; break;
					case prism::PIRinstruction::FUNC_END:    raw_value = "FUNC_END"; break;
					case prism::PIRinstruction::ARG_START:   raw_value = "ARG_START"; break;
					case prism::PIRinstruction::ARG_END:     raw_value = "ARG_END"; break;
					case prism::PIRinstruction::BODY_START:  raw_value = "BODY_START"; break;
					case prism::PIRinstruction::BODY_END:    raw_value = "BODY_END"; break;

					case prism::PIRinstruction::ENTRY:       raw_value = "ENTRY"; break;
					case prism::PIRinstruction::IDENTIF:     raw_value = "IDENTIF"; break;

					case prism::PIRinstruction::INT:         raw_value = "INT"; break;
					case prism::PIRinstruction::LL:          raw_value = "LL"; break;
					case prism::PIRinstruction::FLOAT:       raw_value = "FLOAT"; break;
					case prism::PIRinstruction::DOUBLE:      raw_value = "DOUBLE"; break;

					case prism::PIRinstruction::RET:         raw_value = "RET"; break;

					default:                                 raw_value = "???"; break;
                }
            }

            dump << "[30, 167, 69](" << i << "):\t\t [] [93, 101, 156]Instruction: [][78, 201, 176]" << raw_value << "[]\n";
        }

        ErrorHandler errorhandler;
        errorhandler.log(INFO_PIR_DUMP, compilerconfig, compilerconfig.filepath, dump.str());
    }
}

void prism::PIRBuilder::generatePIR(const Node& node, const CompilerConfig& compilerconfig) {
	switch (node.value.type) {
		case TokenType::FUNC:
			PIRStream.push_back(PIRinstruction::FUNC_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRinstruction::FUNC_END);
			break;

		case TokenType::KEYWORD_ENTRY:
			PIRStream.push_back(PIRinstruction::ENTRY);
			break;

		case TokenType::FUNC_ARGUEMENTS:
			PIRStream.push_back(PIRinstruction::ARG_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRinstruction::ARG_END);
			break;

		case TokenType::FUNC_BODY:
			PIRStream.push_back(PIRinstruction::BODY_START);
			for (const auto& child : node.children) generatePIR(*child, compilerconfig);
			PIRStream.push_back(PIRinstruction::BODY_END);
			break;

		case TokenType::IDENTIFIER:
			PIRStream.push_back(PIRinstruction::IDENTIF);
			PIRStream.push_back(node.value.lexeme);
			break;

		case TokenType::LITERAL_INT:
			PIRStream.push_back(PIRinstruction::INT);
			PIRStream.push_back(std::get<int>(node.value.value));
			break;

		case TokenType::LITERAL_LL:
			PIRStream.push_back(PIRinstruction::LL);
			PIRStream.push_back(std::get<long long>(node.value.value));
			break;

		case TokenType::LITERAL_FLOAT:
			PIRStream.push_back(PIRinstruction::FLOAT);
			PIRStream.push_back(std::get<float>(node.value.value));
			break;

		case TokenType::LITERAL_DOUBLE:
			PIRStream.push_back(PIRinstruction::DOUBLE);
			PIRStream.push_back(std::get<double>(node.value.value));
			break;

		default:
			for (const auto& child : node.children) {
				generatePIR(*child, compilerconfig);
			}
			break;
	}
}
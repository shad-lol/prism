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

#include "include/polariton_ir/pir_unit.hpp"

#include "include/error_handler/error_handler.hpp"

#include <string>

std::string prism::PIRUnit::get_value_str() {
	if (std::holds_alternative<PIRInstruction>(value)) {
		PIRInstruction pinst = std::get<PIRInstruction>(value);
		switch (pinst) {
			case PIRInstruction::FUNC_START: return "FUNC_START"; break;
			case PIRInstruction::FUNC_END: return "FUNC_END"; break;

			case PIRInstruction::ARG_START: return "ARG_START"; break;
			case PIRInstruction::ARG_END: return "ARG_END"; break;

			case PIRInstruction::BODY_START: return "BODY_START"; break;
			case PIRInstruction::BODY_END: return "BODY_END"; break;

			case PIRInstruction::RET: return "RET"; break;

			default: return "Unknown pinst"; break;
		}
	} else if (std::holds_alternative<PIRType>(value)) {
		PIRType ptype = std::get<PIRType>(value);
		switch (ptype) {
			case PIRType::VOID: return "VOID"; break;
			case PIRType::ENTRY: return "ENTRY"; break;

			case PIRType::IDENTIFIER: return "IDENTIF"; break;

			case PIRType::INT: return "INT"; break;
			case PIRType::LONG_LONG: return "LL"; break;
			case PIRType::FLOAT: return "FLOAT"; break;
			case PIRType::DOUBLE: return "DOUBLE"; break;

			default: return "Unknown ptype"; break;
		}
	}
	else if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value);
	else if (std::holds_alternative<int>(value)) return std::to_string(std::get<int>(value));
	else if (std::holds_alternative<long long>(value)) return std::to_string(std::get<long long>(value));
	else if (std::holds_alternative<float>(value)) return std::to_string(std::get<float>(value));
	else return std::to_string(std::get<double>(value));
}
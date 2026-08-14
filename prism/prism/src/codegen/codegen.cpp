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

#include "include/codegen/codegen.hpp"

#include "include/error_handler/error_handler.hpp"

#include <iostream>

prism::CodeGen::CodeGen(std::vector<std::variant<prism::PIRInstruction, int, long long, float, double, std::string>> PIRStream, const CompilerConfig& compilerconfig) {
	this->compilerconfig = compilerconfig;
	this->PIRStream = PIRStream;

	if (compilerconfig.C) C();
	if (compilerconfig.LLVM) LLVM();
}

void prism::CodeGen::C() {
	std::string entryfunc_name;
		
	for (size_t i = 0; i < PIRStream.size(); i++) {
		const auto& pinsti = PIRStream[i];
        if (!std::holds_alternative<PIRInstruction>(pinsti)) {
            std::stringstream data;
            std::visit([&data](const auto& val) { data << val; }, pinsti);
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_NOT_PINST, compilerconfig, data.str());
        }

		const auto& pinst = std::get<PIRInstruction>(pinsti);

		switch (pinst) {
			case PIRInstruction::FUNC_START: {
				PIRInstruction returntype = std::get<PIRInstruction>(PIRStream[++i]); i += 2;
				std::string funcidentifier = std::get<std::string>(PIRStream[i]);

				if (returntype == PIRInstruction::ENTRY) {
					entryfunc_name = funcidentifier;
					Ccode += "int " + funcidentifier;
				}
				else if (returntype == PIRInstruction::VOID) {
					Ccode += "void " + funcidentifier;
				}
				break;
			}

			case PIRInstruction::ARG_START:
				Ccode += "(";
				break;

			case PIRInstruction::ARG_END:
				Ccode += ")";
				break;

			case PIRInstruction::BODY_START:
				Ccode += "{";
				break;

			case PIRInstruction::BODY_END:
				Ccode += "}";
				break;

			case PIRInstruction::RET:
				i++;
				std::stringstream value;
				if (std::holds_alternative<int>(PIRStream[++i])) {
					value << std::get<int>(PIRStream[i]);
				} else if (std::holds_alternative<long long>(PIRStream[++i])) {
					value << std::get<long long>(PIRStream[i]) << "LL";
				} else if (std::holds_alternative<float>(PIRStream[++i])) {
					value << std::get<float>(PIRStream[i]) << "f";
				} else if (std::holds_alternative<double>(PIRStream[++i])) {
					value << std::get<double>(PIRStream[i]);
				}

				Ccode += "return " + value.str() + ";";
		}
    }

	Ccode += "int main(){return " + entryfunc_name + "();}[]";

	if (compilerconfig.dump_c) {
		ErrorHandler errorhandler;
		errorhandler.log(err::INFO_C_DUMP, compilerconfig, compilerconfig.filepath, Ccode);
	}
}

void prism::CodeGen::LLVM() {

}
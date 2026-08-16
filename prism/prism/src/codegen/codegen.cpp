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

prism::CodeGen::CodeGen(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig) {
	if (compilerconfig.C) C(PIRStream, compilerconfig);
	if (compilerconfig.LLVM) LLVM(PIRStream, compilerconfig);
}

void prism::CodeGen::C(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig) {
	std::string entryfunc_name;
		
	for (size_t i = 0; i < PIRStream.size(); i++) {
		PIRUnit pinsti = PIRStream[i];
        if (!pinsti.is_pinst()) {
			ErrorHandler errorhandler;
			errorhandler.log(err::ERROR_NOT_PINST, compilerconfig, pinsti.get_value_str());
        }

		PIRInstruction pinst = std::get<PIRInstruction>(pinsti.get());

		switch (pinst) {
			case PIRInstruction::FUNC_START: {
				PIRType returntype = std::get<PIRType>(PIRStream[++i].get()); i++;
				std::string funcidentifier = std::get<std::string>(PIRStream[++i].get());

				if (returntype == PIRType::ENTRY) {
					entryfunc_name = funcidentifier;
					compilerconfig.appname = funcidentifier;
					Ccode += "int " + funcidentifier;
				}
				else if (returntype == PIRType::VOID) {
					Ccode += "void " + funcidentifier;
				}
				break;
			}

			case PIRInstruction::FUNC_END:
				break;

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
				i += 2;
				std::stringstream value;
				if (PIRStream[i].is_int()) {
					value << std::get<int>(PIRStream[i].get());
				} else if (PIRStream[i].is_long_long()) {
					value << std::get<long long>(PIRStream[i].get()) << "LL";
				} else if (PIRStream[i].is_float()) {
					value << std::get<float>(PIRStream[i].get()) << "f";
				} else if (PIRStream[i].is_double()) {
					value << std::get<double>(PIRStream[i].get());
				}

				Ccode += "return " + value.str() + ";";
		}
    }

	Ccode += "int main(){return " + entryfunc_name + "();}";

	if (compilerconfig.dump_c) {
		ErrorHandler errorhandler;
		errorhandler.log(err::INFO_C_DUMP, compilerconfig, compilerconfig.filepath, "[78, 201, 176]" + Ccode + "[]\n");
	}
}

void prism::CodeGen::LLVM(std::vector<PIRUnit> PIRStream, CompilerConfig& compilerconfig) {

}
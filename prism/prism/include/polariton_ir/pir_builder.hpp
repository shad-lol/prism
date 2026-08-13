#pragma once

#include "pir_instruction.hpp"
#include "include/parser/node.hpp"
#include "include/compiler_config/compiler_config.hpp"

#include <vector>
#include <string>
#include <variant>

namespace prism {

	class PIRBuilder {
	private:
		std::vector<std::variant<prism::PIRinstruction, int, long long, float, double, std::string>> PIRStream;

	public:
		PIRBuilder(const prism::Node& root, const CompilerConfig& compilerconfig);

		void generatePIR(const Node& node, const CompilerConfig& compilerconfig);

		std::vector<std::variant<prism::PIRinstruction, int, long long, float, double, std::string>> get() { return PIRStream; }
	};

}
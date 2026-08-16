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

#pragma once

#include "pir_instruction.hpp"
#include "pir_type.hpp"

#include <variant>

namespace prism {

	class PIRUnit {
	private:
		std::variant<PIRInstruction, PIRType, std::string, int, long long, float, double> value;

	public:
		PIRUnit(PIRInstruction pinst) : value(pinst) {}
		PIRUnit(PIRType ptype) : value(ptype) {}
		PIRUnit(std::string str) : value(str) {}
		PIRUnit(int val) : value(val) {}
		PIRUnit(long long val) : value(val) {}
		PIRUnit(float val) : value(val) {}
		PIRUnit(double val) : value(val) {}

		bool is_pinst() { return std::holds_alternative<PIRInstruction>(value); }
		bool is_ptype() { return std::holds_alternative<PIRType>(value); }
		bool is_string() { return std::holds_alternative<std::string>(value); }
		bool is_int() { return std::holds_alternative<int>(value); }
		bool is_long_long() { return std::holds_alternative<long long>(value); }
		bool is_float() { return std::holds_alternative<float>(value); }
		bool is_double() { return std::holds_alternative<double>(value); }

		std::variant<PIRInstruction, PIRType, std::string, int, long long, float, double> get() { return value; }
		std::string get_value_str();
	};

}
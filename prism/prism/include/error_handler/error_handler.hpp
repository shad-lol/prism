#pragma once

#include "include/lexer/compiler_config.hpp"

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <sstream>

namespace prism {
	enum err : uint32_t {
		SUCCESS =                    0x10000000,
		INFO =                       0x90000000,
		WARNING =                    0xA0000000,
		SYNTAX =                     0xC0000000,
		ERROR =                      0xE0000000,
		FATAL =                      0xF0000000,

		ERROR_NO_ARGUEMENTS =        0xE0000000,
		ERROR_INVALID_COMMAND =      0xE0000001,
		ERROR_INVALID_CONFIG_ID =    0xE0000002,
		ERROR_INVALID_VALUE_CMD =    0xE0000003,

		INFO_SOURCE_DUMP =           0x90000100,
		ERROR_FILE_NOT_FOUND =       0xE0000101,
		ERROR_PERMISSION_DENIED =    0xE0000102,
		ERROR_IS_A_DIRECTORY =       0xE0000103,
		ERROR_NOT_DOT_PRISM =        0xE0000104,
		UKNOWN_FILE_ERROR =          0xE0000105,
		ALERT_MULTIPLE_FILES =       0xA0000106,

		INFO_TOKENS_DUMP =           0x90000200,
		ERROR_MULTIPLE_DOTS =        0xE0000201,
		ERROR_INVALID_TOKEN =        0xE0000202,
		ERROR_UNKNOWN_TOKEN =        0xE0000203,

		INFO_AST_DUMP =              0x90000300,
		SYNTAX_NO_ENTRY =            0xC0000301,
		SYNTAX_ENTRY_ISNT_FUNCTION = 0xC0000302,
		SYNTAX_EXPECTED_IDENTIFIER = 0xC0000303,
		SYNTAX_EXPECTED_SEMICOLON =  0xC0000304,
		SYNTAX_PAREN_NOT_CLOSED =    0xC0000305,
		SYNTAX_BRACES_NOT_CLOSED =   0xC0000306
	};

	class ErrorHandler {
	private:
		std::unordered_map<uint32_t, std::string> ERROR_MAP{
			{err::SUCCESS, "success"},
			{err::INFO, "info"},
			{err::WARNING, "warning"},
			{err::SYNTAX, "syntax error"},
			{err::ERROR, "error"},
			{err::FATAL, "fatal error"},

			{err::ERROR_NO_ARGUEMENTS, "No arguments."},
			{err::ERROR_INVALID_COMMAND, "$command is an invalid command."},
			{err::ERROR_INVALID_CONFIG_ID, "$id is an invalid config id."},
			{err::ERROR_INVALID_VALUE_CMD, "$value is an invalid value for $command command"},

			{err::INFO_SOURCE_DUMP, "$filepath:\n\n$code"},
			{err::ERROR_FILE_NOT_FOUND, "File $path not found."},
			{err::ERROR_PERMISSION_DENIED, "Permission to file $path denied."},
			{err::ERROR_IS_A_DIRECTORY, "$path is a directory."},
			{err::ERROR_NOT_DOT_PRISM, "$path is not a .prism file."},
			{err::UKNOWN_FILE_ERROR, "Couldn't open the file $path for reasons unknown."},
			{err::ALERT_MULTIPLE_FILES, "Multiple file paths entered, every file will use the same compile settings.\n"},

			{err::INFO_TOKENS_DUMP, "$filepath:\n\n$tokens"},
			{err::ERROR_MULTIPLE_DOTS, "floats can't have more than one dot: $float."},
			{err::ERROR_INVALID_TOKEN, "$token is an invalid token."},
			{err::ERROR_UNKNOWN_TOKEN, "$token is an unknown token."},

			{err::INFO_AST_DUMP, "$filepath:\n\n$tree"},
			{err::SYNTAX_NO_ENTRY, "no entry function found."},
			{err::SYNTAX_ENTRY_ISNT_FUNCTION, "the entry point of the program must always be a function."},
			{err::SYNTAX_EXPECTED_IDENTIFIER, "expected identifier."},
			{err::SYNTAX_EXPECTED_SEMICOLON, "expected ;."},
			{err::SYNTAX_PAREN_NOT_CLOSED, "parentheses not closed."},
			{err::SYNTAX_BRACES_NOT_CLOSED, "curly braces not closed."}
		};

		void execute_log(err error_code, CompilerConfig compilerconfig, const std::vector<std::string>& args);

		template<typename T>
		std::string to_str(T val) {
			std::ostringstream ss;
			ss << val;
			return ss.str();
		}

	public:
		template<typename... Args>
		void log(err error_code, CompilerConfig compilerconfig, Args... args) {
			std::vector<std::string> s_args;
			(s_args.push_back(to_str(args)), ...);
			execute_log(error_code, compilerconfig, s_args);
		}

	};
}

#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <sstream>

namespace prism {
	enum err : uint32_t {
		SUCCESS = 0x10000000,
		INFO = 0x90000000,
		WARNING = 0xA0000000,
		ERROR = 0xE0000000,
		FATAL = 0xF0000000,

		ERROR_NO_ARGUEMENTS = 0xE0000000,
		ERROR_INVALID_COMMAND = 0xE0000001,
		ERROR_INVALID_CONFIG_ID = 0xE0000002,

		ERROR_FILE_NOT_FOUND = 0xE0000100,
		ERROR_PERMISSION_DENIED = 0xE0000101,
		ERROR_IS_A_DIRECTORY = 0xE0000102,
		ERROR_NOT_DOT_PRISM = 0xE0000103,
		UKNOWN_FILE_ERROR = 0xE0000104,
		ALERT_MULTIPLE_FILES = 0xA0000105,
	};

	class ErrorHandler {
	private:
		std::unordered_map<uint32_t, std::string> ERROR_MAP{
			{err::SUCCESS, "success"},
			{err::INFO, "info"},
			{err::WARNING, "warning"},
			{err::ERROR, "error"},
			{err::FATAL, "fatal error"},

			{err::ERROR_NO_ARGUEMENTS, "No arguments"},
			{err::ERROR_INVALID_COMMAND, "$cmd is an invalid command"},
			{err::ERROR_INVALID_CONFIG_ID, "Invalid config id: $id"},

			{err::ERROR_FILE_NOT_FOUND, "File $path not found."},
			{err::ERROR_PERMISSION_DENIED, "Permission to file $path denied."},
			{err::ERROR_IS_A_DIRECTORY, "$path is a directory."},
			{err::ERROR_NOT_DOT_PRISM, "$path is not a .prism file."},
			{err::UKNOWN_FILE_ERROR, "Couldn't open the file $path for reasons unknown"},
			{err::ALERT_MULTIPLE_FILES, "Multiple file paths entered, every file will use the same compile settings"}
		};

		void execute_log(uint32_t error_code, const std::vector<std::string>& args);

		template<typename T>
		std::string to_str(T val) {
			std::ostringstream ss;
			ss << val;
			return ss.str();
		}

	public:
		template<typename... Args>
		void log(uint32_t error_code, Args... args) {
			std::vector<std::string> s_args;
			(s_args.push_back(to_str(args)), ...);
			execute_log(error_code, s_args);
		}

	};
}

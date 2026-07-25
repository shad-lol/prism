#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

#include "include/logger/logger.hpp"

namespace prism {

	enum err : uint32_t {
		SUCCESS =                 0x10000000,
		INFO =                    0x90000000,
		WARNING =                 0xA0000000,
		ERROR =                   0xE0000000,
		FATAL =	                  0xF0000000,

		ERROR_NO_ARGUEMENTS =     0xE0000000,

		ERROR_FILE_NOT_FOUND =    0xE0000100,
		ERROR_PERMISSION_DENIED = 0xE0000101,
		ERROR_IS_A_DIRECTORY =    0xE0000102,
		ERROR_NOT_DOT_PRISM =     0xE0000103,
		UKNOWN_FILE_ERROR =       0xE0000104,
	};

	class ErrorHandler {
	private:
		std::unordered_map<uint32_t, std::string> ERROR_MAP{
			{err::SUCCESS, "[40, 167, 69]success.\n"},
			{err::INFO, "[33, 150, 243]info.\n"},
			{err::WARNING, "[255, 152, 0]warning.\n"},
			{err::ERROR, "[220, 53, 69]error.\n"},
			{err::FATAL, "[139, 0, 0]fatal error.\n"},

			{err::ERROR_NO_ARGUEMENTS, "[240, 240, 240]No arguements.\n"},

			{err::ERROR_FILE_NOT_FOUND, "[240, 240, 240]File not found.\n"},
			{err::ERROR_PERMISSION_DENIED, "[240, 240, 240]Permission to file denied.\n"},
			{err::ERROR_IS_A_DIRECTORY, "[240, 240, 240].This is a directory: "},
			{err::ERROR_NOT_DOT_PRISM, "[240, 240, 240]The file is not a .prism file: "},
			{err::UKNOWN_FILE_ERROR, "[240, 240, 240]Couldn't open the file for reasons unknown: "}
		};

	public:
		void log(uint32_t error_code);
	};

}
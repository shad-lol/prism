#include <iostream>
#include <iomanip>
#include <sstream>

#include "include/error_handler/error_handler.hpp"

void prism::ErrorHandler::log(uint32_t error_code) {
	prism::Logger logger;
	uint32_t status = error_code >> 28;

	switch (status) {
	case 0x1:
		logger.cerrrgb("[40, 167, 69]success");
		break;
	case 0x9:
		logger.cerrrgb("[33, 150, 243]info");
		break;
	case 0xA:
		logger.cerrrgb("[255, 152, 0]warning");
		break;
	case 0xE:
		logger.cerrrgb("[220, 53, 69]error");
		break;
	case 0xF:
		logger.cerrrgb("[139, 0, 0]fatal error");
		break;
	default:
		logger.cerrrgb("[108, 117, 125]unknown");
		break;
	}

	std::ostringstream log;
	log << "(" << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << error_code << "): ";

	logger.cerrrgb(log.str());
	logger.cerrrgb(ERROR_MAP[error_code]);
}
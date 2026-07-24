#include "include/.prism_reader/.prism_reader.hpp"

prism::PrismReader::PrismReader(std::string filepath) {
	ifstream_code.open(filepath);
	if (!ifstream_code.is_open()) {
		std::cerr << "error: Could not open file at path: " << filepath << '\n';
	}
	ostringstream_code << ifstream_code.rdbuf();
	string_code = ostringstream_code.str();
}

prism::PrismReader::~PrismReader() {
	if (ifstream_code.is_open()) {
		ifstream_code.close();
	}
}

std::string_view prism::PrismReader::get() {
	return string_code;
}

void prism::PrismReader::dump() {
	std::cout << string_code << std::endl;
}

bool prism::PrismReader::is_open() {
	return ifstream_code.is_open();
}
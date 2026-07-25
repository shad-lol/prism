#include "include/.prism_reader/.prism_reader.hpp"

int main(int argc, char* argv[]) {
	int filecount = 1;
	std::string filepath = argv[1];

	for (int i = 1; i <= filecount; i++) {
		prism::PrismReader reader(filepath);
		if (!reader.is_open()) return 1;

		reader.dump();
		std::string_view code = reader.get();
	}
}
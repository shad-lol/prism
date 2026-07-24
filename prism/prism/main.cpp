#include "include/.prism_reader/.prism_reader.hpp"

int main() {
	std::string filepath = "D:/VSProjects/prism/prism/examples/01_the_basics.prism";
//	std::cin >> filepath;

	prism::PrismReader reader(filepath);
	if (!reader.is_open()) return 1;

	std::string_view code = reader.get();
}
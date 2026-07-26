#include "include/.prism_reader/.prism_reader.hpp"
#include "include/lexer/arg_reader.hpp"

int main(int argc, char* argv[]) {
	prism::ArgReader argreader(argc, argv);

	size_t filecount = argreader.filecount();
	for (int i = 0; i < filecount; i++) {
		prism::PrismReader prismreader(argreader.file(i));

		if (argreader.getconfig_bool(prism::ConfigId::DumpPrism) && prismreader.is_open()) prismreader.dump();
	}
}
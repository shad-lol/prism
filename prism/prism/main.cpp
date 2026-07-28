#include "include/lexer/arg_reader.hpp"
#include "include/.prism_reader/.prism_reader.hpp"
#include "include/lexer/lexer.hpp"

int main(int argc, char* argv[]) {
	prism::CompilerConfig compilerconfig;
	prism::ArgReader argreader(argc, argv, compilerconfig);

	size_t filecount = argreader.filecount();
	for (int i = 0; i < filecount; i++) {
		compilerconfig.filepath = argreader.file(i);
		prism::PrismReader prismreader(argreader.file(i), compilerconfig);
		std::string_view code = prismreader.get();
		prism::Lexer lexer(code, compilerconfig);
	}
}
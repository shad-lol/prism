#ifdef _WIN32
	#include <Windows.h>

	#ifdef ERROR
		#undef ERROR
	#endif

	#ifdef ERROR_FILE_NOT_FOUND
		#undef ERROR_FILE_NOT_FOUND
	#endif

	#ifdef ERROR_INVALID_TOKEN
		#undef ERROR_INVALID_TOKEN
	#endif
#endif

#include "include/lexer/arg_reader.hpp"
#include "include/.prism_reader/.prism_reader.hpp"
#include "include/lexer/lexer.hpp"
#include "include/parser/parser.hpp"

int main(int argc, char* argv[]) {

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	prism::CompilerConfig compilerconfig;
	prism::ArgReader argreader(argc, argv, compilerconfig);

	size_t filecount = argreader.filecount();
	for (int i = 0; i < filecount; i++) {
		compilerconfig.filepath = argreader.file(i);
		prism::PrismReader prismreader(argreader.file(i), compilerconfig);

		std::string_view code = prismreader.get();
		prism::Lexer lexer(code, compilerconfig);

		std::vector<prism::Token> tokens = lexer.get();
		prism::Parser parser(tokens, compilerconfig);
	}
}
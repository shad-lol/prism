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

#include "include/arg_reader/arg_reader.hpp"
#include "include/.prism_reader/.prism_reader.hpp"
#include "include/lexer/lexer.hpp"
#include "include/parser/parser.hpp"
#include "include/polariton_ir/pir_builder.hpp"

int main(int argc, char* argv[]) {

#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	prism::CompilerConfig compilerconfig;
	prism::ArgReader argreader(argc, argv, compilerconfig);

	for (size_t i = 0; i < argreader.filecount(); i++) {
		compilerconfig.filepath = argreader.file(i);

		prism::PrismReader prismreader(compilerconfig);
		prism::Lexer lexer(prismreader.get(), compilerconfig);
		prism::Parser parser(lexer.get(), compilerconfig);
		prism::PIRBuilder(parser.get(), compilerconfig);
	}
}
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

namespace prism {

	enum ConfigId : std::uint8_t {
		DumpPrism = 0,
	};

	struct CompileConfig {
		bool dump_prism = false;
	};

	class ArgReader {

	private:
		std::vector<std::string> arguements;
		std::vector<std::string> files;
		CompileConfig compileconfig;

	public:
		ArgReader(int argc, char* argv[]);

		size_t filecount();
		std::string file(int file_id) { return prism::ArgReader::files[file_id]; }

		using ConfigValue = std::variant<bool, std::string>;
		ConfigValue getconfig_wrapper(ConfigId id) const;

		bool getconfig_bool(ConfigId id) const { return std::get<bool>(getconfig_wrapper(id)); };
	};

}
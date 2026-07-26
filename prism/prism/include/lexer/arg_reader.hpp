#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <bitset>

namespace prism {

	enum flags : uint8_t {
		build = 1,
		run,
		prism_dump
	};

	class ArgReader {
	public:
		using ConfigBits = std::bitset<256>;

	private:
		std::vector<std::string> arguements;
		std::vector<std::string> files;
		ConfigBits compilerconfig;

	public:
		ArgReader(int argc, char* argv[]);

		size_t filecount();
		std::string file(int file_id) { return prism::ArgReader::files[file_id]; }

		const ConfigBits& get_bits() const { return compilerconfig; }

		void toggle(size_t index) { compilerconfig[index].flip(); }
		void enable(size_t index) { compilerconfig[index] = true; }
		void enableall(size_t index) { compilerconfig.set(); }

		void disable(size_t index) { compilerconfig[index] = false; }
		void disableall(size_t index) { compilerconfig.reset(); }

		bool check(size_t index) { return compilerconfig[index]; }

		void process();
	};

}
#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <format>
#include <print>
#include <iostream>
#include <charconv>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

struct default_handler { void operator()(uint16_t error_code) const { std::exit(error_code); } };

template <typename T>
concept Error = requires {
    typename T::value_type;
    typename T::error_type;
} && std::same_as<typename T::error_type, uint16_t>;

template <typename F = default_handler>
requires std::invocable<F, uint16_t>
inline void verify(uint16_t err, F&& handler = F{}) {
    if (err != 0)
        std::forward<F>(handler)(err);
}

template <Error E, typename F = default_handler>
requires std::invocable<F, uint16_t>
inline void verify(const E& err, F&& handler = F{}) {
    if (!err)
        std::forward<F>(handler)(err.error());
}

class Diagnostics {

public:
    bool silent = false;

    Diagnostics() {
        registry[0xF001] = "[100, 181, 246][Diagnostics][/] Unknown code: [100, 181, 246]0x${code}[/].";
        registry[0xF002] = "[100, 181, 246][Diagnostics][/] Format error for code [100, 181, 246]0x${code}[/]: Wrong argument count.";
        registry[0xA003] = "[100, 181, 246][Diagnostics][/] Code [100, 181, 246]0x${code}[/] is already registered.";

        #ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
            DWORD dwMode = 0;

            if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
                SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
            if (hErr != INVALID_HANDLE_VALUE && GetConsoleMode(hErr, &dwMode)) {
                SetConsoleMode(hErr, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        #endif
    }

    void register_code(uint32_t hex_code, std::string_view text_template) {
        if (registry.contains(hex_code)) {
            char buffer[9];
            auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), hex_code, 16);

            for (char* p = buffer; p < ptr; ++p) {
                if (*p >= 'a' && *p <= 'f') {
                    *p -= 32;
                }
            }

            log(0xA003, std::string_view(buffer, ptr - buffer));
        }

        registry[hex_code] = std::string(text_template);
    }

    template<typename... Args>
    uint16_t log(uint16_t hex_code, Args&&... args) const {
        auto it = registry.find(hex_code);

        if (it == registry.end()) {
            char buffer[9];
            auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), hex_code, 16);

            for (char* p = buffer; p < ptr; ++p) {
                if (*p >= 'a' && *p <= 'f') {
                    *p -= 32;
                }
            }

            return log(0xF001, std::string_view(buffer, ptr - buffer));
        }

        std::string text = replace_variables(it->second);
        std::string message;
        try {
            message = std::vformat(text, std::make_format_args(args...));
        }
        catch (...) {
            char buffer[9];
            auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), hex_code, 16);

            for (char* p = buffer; p < ptr; ++p) {
                if (*p >= 'a' && *p <= 'f') {
                    *p -= 32;
                }
            }

            return log(0xF002, std::string_view(buffer, ptr - buffer));
        }

        message = parse_colors(message) + "\033[0m";
        uint8_t severity = hex_code >> 12;

        if (severity == 1) {
            if (!silent)
                std::println(std::cout, "{}", message);
            return 0x1000;
        }

        std::string prefix = generate_prefix(severity, hex_code);
        std::ostream& os = (severity == 0xE || severity == 0xF) ? std::cerr : std::cout;

        if (!silent)
            std::println(os, "{}{}", prefix, message);

        if (severity == 0xF)
            std::exit(hex_code);

        return hex_code;
    }

private:
    std::unordered_map<uint32_t, std::string> registry;

    static std::string replace_variables(std::string text) {
        size_t pos = 0;
        while ((pos = text.find("${", pos)) != std::string::npos) {
            if (size_t end = text.find('}', pos); end != std::string::npos) {
                text.replace(pos, end - pos + 1, "{}");
                pos += 2;
            }
            else break;
        }
        return text;
    }

    static std::string parse_colors(std::string_view text) {
        std::string result;
        result.reserve(text.size() * 2);

        size_t i = 0;
        while (i < text.size()) {
            if (text.substr(i).starts_with("[/]")) {
                result += "\033[0m";
                i += 3;
                continue;
            }

            if (text[i] == '[') {
                size_t close_bracket = text.find(']', i);
                if (close_bracket != std::string_view::npos) {
                    std::string_view inside = text.substr(i + 1, close_bracket - i - 1);

                    int r = 0, g = 0, b = 0;
                    size_t sub_idx = 0;

                    auto read_num = [&](size_t& idx) -> int {
                        while (idx < inside.size() && inside[idx] == ' ') idx++;
                        int val = 0; bool has_digits = false;
                        while (idx < inside.size() && inside[idx] >= '0' && inside[idx] <= '9') {
                            val = val * 10 + (inside[idx] - '0');
                            idx++; has_digits = true;
                        }
                        while (idx < inside.size() && inside[idx] == ' ') idx++;
                        return has_digits ? val : -1;
                        };

                    r = read_num(sub_idx);
                    bool ok1 = (sub_idx < inside.size() && inside[sub_idx++] == ',');
                    g = read_num(sub_idx);
                    bool ok2 = (sub_idx < inside.size() && inside[sub_idx++] == ',');
                    b = read_num(sub_idx);

                    if (r >= 0 && g >= 0 && b >= 0 && ok1 && ok2 && sub_idx == inside.size()) {
                        std::format_to(std::back_inserter(result), "\033[38;2;{};{};{}m", r, g, b);
                        i = close_bracket + 1;
                        continue;
                    }
                }
            }

            result += text[i++];
        }
        return result;
    }

    static std::string generate_prefix(uint8_t severity, uint32_t code) {
        std::string_view name;
        std::string_view color;

        switch (severity) {
            case 0x0: name = "success";      color = "[34, 206, 91]";   break;
            case 0x1: name = "text";         color = "[240, 240, 240]"; break;
            case 0x9: name = "info";         color = "[0, 190, 255]";   break;
            case 0xA: name = "warning";      color = "[249, 107, 6]";   break;
            case 0xE: name = "error";        color = "[238, 43, 43]";   break;
            case 0xC: name = "syntax error"; color = "[233, 63, 37]";   break;
            case 0xF: name = "fatal error";  color = "[189, 15, 15]";   break;
            default:  name = "unknown";      color = "[128, 128, 128]"; break;
        }

        char buffer[9];
        auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), code, 16);

        for (char* p = buffer; p < ptr; ++p) {
            if (*p >= 'a' && *p <= 'f') {
                *p -= 32;
            }
        }

        std::string prefix = std::format("{}{}[/]([100, 181, 246]0x{:0>4}[/]): ", color, name, std::string_view(buffer, ptr - buffer));

        return parse_colors(prefix);
    }
};
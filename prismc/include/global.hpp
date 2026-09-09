
/*   Copyright 2026 Prism Compiler Authors
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once

#ifdef PRISMC_EXPORTS
    #if defined(_MSC_VER) || defined(__CYGWIN__)
        #define PRISMC_API __declspec(dllexport)
    #elif defined(__GNUC__)
        #define PRISMC_API __attribute__((visibility("default")))
    #else
        #define PRISMC_API
    #endif
#else
    #if defined(_MSC_VER) || defined(__CYGWIN__)
        #define PRISMC_API __declspec(dllimport)
    #else
        #define PRISMC_API
    #endif
#endif

#include <diagnostics.hpp>

#include <cstdint>

namespace prismc {

    namespace err {
        enum type : std::uint16_t {

            TXT = 0x0000,
            SUCCESS = 0x1000,
            INFO = 0x9000,
            WARN = 0xA000,
            ERR = 0xE000,
            FATAL = 0xF000,

            ERR_FILE_NOT_FOUND = 0xE100,
            ERR_ACCESS_DENIED = 0xE101,
            ERR_UNKNOWN_FILE_ERROR = 0xE102,

        };
    }

    extern PRISMC_API Diagnostics diag;

    inline void register_codes() {

        diag.register_code(err::TXT, "text");
        diag.register_code(err::SUCCESS, "success");
        diag.register_code(err::INFO, "info");
        diag.register_code(err::WARN, "warning");
        diag.register_code(err::ERR, "error");
        diag.register_code(err::FATAL, "fatal error");

        diag.register_code(err::ERR_FILE_NOT_FOUND, "failed to find file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ERR_ACCESS_DENIED, "access denied for file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ERR_UNKNOWN_FILE_ERROR, "could not open file [230, 192, 123]${filepath}[/] due to an unknown error.");

    }

};
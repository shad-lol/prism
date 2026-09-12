
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

namespace prismc {

    namespace err {
        enum type : uint16_t {

            Text = 0x0000,
            Success = 0x1000,
            Info = 0x9000,
            Warn = 0xA000,
            Err = 0xE000,
            Fatal = 0xF000,

            ErrFileNotFound = 0xE100,
            ErrAccessDenied = 0xE101,
            ErrUnknownFileError = 0xE102,

        };
    }

    extern PRISMC_API Diagnostics diag;

    inline void register_codes() {

        diag.register_code(err::Text, "text");
        diag.register_code(err::Success, "success");
        diag.register_code(err::Info, "info");
        diag.register_code(err::Warn, "warning");
        diag.register_code(err::Err, "error");
        diag.register_code(err::Fatal, "fatal error");

        diag.register_code(err::ErrFileNotFound, "failed to find file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ErrAccessDenied, "access denied for file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ErrUnknownFileError, "could not open file [230, 192, 123]${filepath}[/] due to an unknown error.");

    }

};
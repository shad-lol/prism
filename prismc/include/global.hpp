
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

#include "diagnostics.hpp"

namespace prismc {

    namespace err {
        enum type : uint16_t {

            Success = 0x0000,
            Text = 0x1000,
            Info = 0x9000,
            Warn = 0xA000,
            Err = 0xE000,
            Fatal = 0xF000,

            FatalFileNotFound = 0xF101,
            ErrAccessDenied = 0xE102,
            ErrUnknownFileError = 0xE103,

            SyntaxUnclosedComment = 0xC201,
            SyntaxUnclosedStringLiteral = 0xC202,
            SyntaxInvalidToken = 0xC203,

            SyntaxRedifinition = 0xC301,
            SyntaxUndeclaredIdentifier = 0xC302,
            SyntaxExpectedToken = 0xC303,
            SyntaxExpectedStatement = 0xC304,
            SyntaxExpectedExpression = 0xC305,
            SyntaxExpectedIdentifier = 0xC306,
            SyntaxExpectedEndOfLine = 0xC307,
            FatalFuncNotFound = 0xF308,
            SyntaxIdentifierNotGiven = 0xC309,
            SyntaxExpectedFuncKeyword = 0xC30A,
            SyntaxCustomRetTypeForEntry = 0xC30B,

            ErrLLVMError = 0xE401,

            ErrTargetMachineInitError = 0xE501,
            ErrNotInitialized = 0xE502,
            ErrEmitFileUnsupported = 0xE503,
            ErrBufferCreationFailed = 0xE504,
            ErrObjectParsingFailed = 0xE505,
            ErrOutputFileError = 0xE506,

            ErrLinkError = 0xE601,

        };
    }

    extern PRISMC_API Diagnostics diag;

    inline int register_codes() {

        diag.register_code(err::Success, "success");
        diag.register_code(err::Text, "text");
        diag.register_code(err::Info, "info");
        diag.register_code(err::Warn, "warning");
        diag.register_code(err::Err, "error");
        diag.register_code(err::Fatal, "fatal error");

        diag.register_code(err::FatalFileNotFound, "failed to find file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ErrAccessDenied, "access denied for file: [230, 192, 123]${filepath}[/].");
        diag.register_code(err::ErrUnknownFileError, "could not open file [230, 192, 123]${filepath}[/] due to an unknown error.");

        diag.register_code(err::SyntaxUnclosedComment, "${location}: unclosed comment.");
        diag.register_code(err::SyntaxUnclosedStringLiteral, "${location}: unclosed string literal.");
        diag.register_code(err::SyntaxInvalidToken, "${location}: invalid token.");

        diag.register_code(err::SyntaxRedifinition, "${location}: redifinition of ${type} [245, 218, 163]${signature}[/].");
        diag.register_code(err::SyntaxUndeclaredIdentifier, "${location}: identifier \"${identifier}\" is undefined.");
        diag.register_code(err::SyntaxExpectedToken, "${location}: expected '${token}' token.");
        diag.register_code(err::SyntaxExpectedStatement, "${location}: expected statement.");
        diag.register_code(err::SyntaxExpectedExpression, "${location}: expected expression.");
        diag.register_code(err::SyntaxExpectedIdentifier, "${location}: expected identifier.");
        diag.register_code(err::SyntaxExpectedEndOfLine, "${location}: expected end of line.");
        diag.register_code(err::FatalFuncNotFound, "could not find function with given signature '${signature}'.");
        diag.register_code(err::SyntaxIdentifierNotGiven, "${location}: identifier not given during ${type} declaration.");
        diag.register_code(err::SyntaxExpectedFuncKeyword, "${location}: expected 'func' keyword after function modifiers.");
        diag.register_code(err::SyntaxCustomRetTypeForEntry, "${location}: cannot assign a custom return type '${type}' to entry function '${func_name}'.");

        diag.register_code(err::ErrLLVMError, "LLVM error: ${error}");

        diag.register_code(err::ErrTargetMachineInitError, "failed to initialize target machine: ${error}.");
        diag.register_code(err::ErrNotInitialized, "usage of uninitialized object compiler.");
        diag.register_code(err::ErrEmitFileUnsupported, "selected target machine configuration does not support direct binary object code generation.");
        diag.register_code(err::ErrBufferCreationFailed, "failed to create memory buffer for a binary object.");
        diag.register_code(err::ErrObjectParsingFailed, "failed to parse a binary object.");
        diag.register_code(err::ErrOutputFileError, "failed to create or open the output object file.");

        diag.register_code(err::ErrLinkError, "link error.");

        return 0;

    }

};
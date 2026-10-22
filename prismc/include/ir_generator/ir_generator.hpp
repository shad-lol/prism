
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

#include <global.hpp>

#include <parser/ast.hpp>

#ifdef __INTELLISENSE__
#undef _MSC_VER
#define _MSC_VER 1951 
#undef __clang__
#endif

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include <expected>
#include <map>

namespace prismc {

    class PRISMC_API IRGenerator {

    public:
        IRGenerator();

        std::expected<std::unique_ptr<llvm::Module>, uint16_t> compile(const AST& ast,
            const llvm::DataLayout& data_layout, const llvm::Triple& triple
        );

    private:
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;

        std::vector<llvm::Type*> types;

        struct Symbol {
            llvm::Value* address;
            llvm::Type* type;
        };
        std::map<std::string, Symbol> symbol_table;

        void gen_types(const std::vector<Struct>& structs);

        void gen_func_prototype(llvm::Module& module, const Func& func);
        void gen_func_body(llvm::Module& module, const Func& func);

        void gen_body(const Body& body);
        void gen_statement(const Statement& statement);
        llvm::Value* gen_expression(const Expression& expression);

        llvm::Value* gen_call_node(const CallNode& call);
        llvm::Value* gen_intrinsic_node(const std::string_view& name, const CallNode& call);

    };

}
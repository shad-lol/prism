
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

#include <ir_generator/ir_generator.hpp>

#include <llvm/IR/Verifier.h>

#include <filesystem>
#include <variant>

namespace prismc {

    IRGenerator::IRGenerator() : builder(context) {}

    std::expected<std::unique_ptr<llvm::Module>, uint16_t> IRGenerator::compile(const AST& ast,
        const llvm::DataLayout& data_layout, const llvm::Triple& triple
    ) {
        auto module = std::make_unique<llvm::Module>(ast.path->string(), context);
        module->setDataLayout(data_layout);
        module->setTargetTriple(triple);

        gen_types(ast.structs);

        for (const auto& func : ast.funcs)
            gen_func_prototype(*module, func);

        for (const auto& func : ast.funcs)
            gen_func_body(*module, func);

        std::string error;
        llvm::raw_string_ostream os(error);
        if (llvm::verifyModule(*module, &os)) {
            module->print(llvm::outs(), nullptr);
            return std::unexpected(diag.log(err::ErrLLVMError, error));
        }

        return module;
    }

    void IRGenerator::gen_types(const std::vector<Struct>& structs) {
        types.clear();
        types.reserve(structs.size());

        for (auto const& struct_ : structs) {
            if (struct_.identifier == "Void") {
                types.push_back(builder.getVoidTy());
                continue;
            }

            std::string identifier = struct_.identifier;

            if (identifier == "Int8" || identifier == "UInt8")        types.push_back(builder.getInt8Ty());
            else if (identifier == "Int16" || identifier == "UInt16") types.push_back(builder.getInt16Ty());
            else if (identifier == "Int32" || identifier == "UInt32") types.push_back(builder.getInt32Ty());
            else if (identifier == "Int64" || identifier == "UInt64") types.push_back(builder.getInt64Ty());

            else if (identifier == "Float32") types.push_back(builder.getFloatTy());
            else if (identifier == "Float64") types.push_back(builder.getDoubleTy());

            continue;

            llvm::StructType* struct_type = llvm::StructType::create(context, struct_.identifier);
            types.push_back(struct_type);
        }

        for (size_t id = 0; id < structs.size(); id++) {
            const auto& struct_ = structs[id];

            if (!struct_.identifier.empty()) {
                static const std::unordered_set<std::string_view> builtin_types = {
                    "Void",
                    "Int8", "UInt8", "Int16", "UInt16",
                    "Int32", "UInt32", "Int64", "UInt64",
                    "Float32", "Float64"
                };

                if (builtin_types.find(struct_.identifier) == builtin_types.end()) {
                    llvm::StructType* struct_type = llvm::cast<llvm::StructType>(types[id]);

                    std::vector<llvm::Type*> member_types;
                    member_types.reserve(struct_.layout.size());

                    for (const auto& field : struct_.layout)
                        member_types.push_back(types[field.typeID]);

                    struct_type->setBody(member_types);
                }
            }
        }
    }

    void IRGenerator::gen_func_prototype(llvm::Module& module, const Func& func) {
        llvm::Type* return_type = types[func.signature.return_typeID];

        std::vector<llvm::Type*> param_types;
        param_types.reserve(func.signature.parameters.size());

        for (const auto& param : func.signature.parameters)
            param_types.push_back(types[param.typeID]);

        llvm::FunctionType* func_type = llvm::FunctionType::get(return_type, param_types, false);
        std::string llvm_name = (func.signature.modifiers[Entry]) ? "main" : func.signature.identifier;

        llvm::Function::Create(
            func_type,
            llvm::Function::ExternalLinkage,
            llvm_name,
            &module
        );
    }

    void IRGenerator::gen_func_body(llvm::Module& module, const Func& func) {
        std::string llvm_name = (func.signature.modifiers[Entry]) ? "main" : func.signature.identifier;

        llvm::Function* llvm_func = module.getFunction(llvm_name);

        llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(context, "entry", llvm_func);
        builder.SetInsertPoint(entry_block);

        symbol_table.clear();

        size_t idx = 0;
        for (auto& llvm_arg : llvm_func->args()) {
            const auto& param = func.signature.parameters[idx];
            std::string param_name = param.label;
            llvm::Type* param_type = types[param.typeID];

            llvm::Value* alloca_inst = builder.CreateAlloca(param_type, nullptr, param_name);
            builder.CreateStore(&llvm_arg, alloca_inst);

            symbol_table[param_name] = Symbol{ alloca_inst, param_type };
            idx++;
        }

        gen_body(func.body);
    }

    void IRGenerator::gen_body(const Body& body) {
        for (const auto& statement : body.statements) {
            if (builder.GetInsertBlock()->getTerminator() != nullptr)
                break;
            gen_statement(statement);
        }
    }

    void IRGenerator::gen_statement(const Statement& statement) {
        if (std::holds_alternative<ReturnNode>(statement.statement)) {
            const auto& ret = std::get<ReturnNode>(statement.statement);

            llvm::Value* expr_val = gen_expression(ret.expression);

            if (!expr_val || expr_val->getType()->isVoidTy())
                builder.CreateRetVoid();
            else
                builder.CreateRet(expr_val);
        }
        else if (std::holds_alternative<std::vector<VarDeclNode>>(statement.statement)) {
            const auto& variables = std::get<std::vector<VarDeclNode>>(statement.statement);

            for (const auto& var_decl : variables) {
                llvm::Value* init_val = gen_expression(var_decl.initializer);
                if (!init_val) return;

                llvm::Type* var_type = init_val->getType();

                llvm::Value* alloca_inst = builder.CreateAlloca(var_type, nullptr, var_decl.identifier);

                builder.CreateStore(init_val, alloca_inst);

                symbol_table[var_decl.identifier] = Symbol{ alloca_inst, var_type };
            }
        }
        else if (std::holds_alternative<CallNode>(statement.statement)) {
            const auto& call = std::get<CallNode>(statement.statement);
            gen_call_node(call);
        }
        else if (std::holds_alternative<std::shared_ptr<Body>>(statement.statement)) {
            const auto& inner_body = std::get<std::shared_ptr<Body>>(statement.statement);

            if (inner_body)
                gen_body(*inner_body);
        }
    }

    llvm::Value* IRGenerator::gen_expression(const Expression& expr) {
        if (std::holds_alternative<std::monostate>(expr.value))
            return nullptr;

        if (std::holds_alternative<int8_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt8Ty(), std::get<int8_t>(expr.value));
        if (std::holds_alternative<int16_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt16Ty(), std::get<int16_t>(expr.value));
        if (std::holds_alternative<int32_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt32Ty(), std::get<int32_t>(expr.value));
        if (std::holds_alternative<int64_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt64Ty(), std::get<int64_t>(expr.value));

        if (std::holds_alternative<uint8_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt8Ty(), std::get<uint8_t>(expr.value), false);
        if (std::holds_alternative<uint16_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt16Ty(), std::get<uint16_t>(expr.value), false);
        if (std::holds_alternative<uint32_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt32Ty(), std::get<uint32_t>(expr.value), false);
        if (std::holds_alternative<uint64_t>(expr.value))
            return llvm::ConstantInt::get(builder.getInt64Ty(), std::get<uint64_t>(expr.value), false);

        if (std::holds_alternative<float>(expr.value))
            return llvm::ConstantFP::get(builder.getFloatTy(), std::get<float>(expr.value));
        if (std::holds_alternative<double>(expr.value))
            return llvm::ConstantFP::get(builder.getDoubleTy(), std::get<double>(expr.value));

        if (std::holds_alternative<bool>(expr.value))
            return llvm::ConstantInt::get(builder.getInt1Ty(), std::get<bool>(expr.value));

        if (std::holds_alternative<std::string>(expr.value))
            return builder.CreateGlobalString(std::get<std::string>(expr.value), "str_literal");

        if (std::holds_alternative<std::shared_ptr<VariableNode>>(expr.value)) {
            const auto& var_node = std::get<std::shared_ptr<VariableNode>>(expr.value);

            auto it = symbol_table.find(var_node->identifier);
            if (it == symbol_table.end())
                return nullptr;

            const Symbol& sym = it->second;

            return builder.CreateLoad(sym.type, sym.address, var_node->identifier + "_load");
        }

        if (std::holds_alternative<std::shared_ptr<CallNode>>(expr.value)) {
            const auto& call_node = std::get<std::shared_ptr<CallNode>>(expr.value);
            if (!call_node) return nullptr;

            return gen_call_node(*call_node);
        }

        return nullptr;
    }

    llvm::Value* IRGenerator::gen_call_node(const CallNode& call) {
        llvm::Module* module = builder.GetInsertBlock()->getModule();

        std::string target_name = call.callee;

        if (!target_name.empty() && target_name[0] == '@')
            return gen_intrinsic_node(target_name, call);

        llvm::Function* callee_func = module->getFunction(target_name);

        if (!callee_func && target_name == "main")
            callee_func = module->getFunction("main");

        if (!callee_func) return nullptr;

        std::vector<llvm::Value*> args_val;
        args_val.reserve(call.arguments.size());

        for (const auto& arg : call.arguments) {
            llvm::Value* arg_val = gen_expression(arg.value);
            if (!arg_val) return nullptr;
            args_val.push_back(arg_val);
        }

        return builder.CreateCall(callee_func->getFunctionType(), callee_func, args_val);
    }

    llvm::Value* IRGenerator::gen_intrinsic_node(const std::string_view& name, const CallNode& call) {
        using BinaryOp = std::function<llvm::Value* (llvm::Value*, llvm::Value*, llvm::IRBuilder<>&)>;

        static const std::unordered_map<std::string_view, BinaryOp> intrinsics = {
            {"@i32_add", [](auto* l, auto* r, auto& b) { return b.CreateAdd(l, r, "add_tmp"); }},
            {"@i32_sub", [](auto* l, auto* r, auto& b) { return b.CreateSub(l, r, "sub_tmp"); }},
            {"@i32_mul", [](auto* l, auto* r, auto& b) { return b.CreateMul(l, r, "mul_tmp"); }},
            {"@i32_div", [](auto* l, auto* r, auto& b) { return b.CreateSDiv(l, r, "div_tmp"); }},
        };

        auto it = intrinsics.find(name);
        if (it == intrinsics.end()) return nullptr;

        if (call.arguments.size() != 2) return nullptr;

        llvm::Value* lhs = gen_expression(call.arguments[0].value);
        llvm::Value* rhs = gen_expression(call.arguments[1].value);
        if (!lhs || !rhs) return nullptr;

        return it->second(lhs, rhs, builder);
    }

}
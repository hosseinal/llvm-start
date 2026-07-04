#ifndef HLLVM_H
#define HLLVM_H


#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include "parser/EvaParser.h"

#include <memory>
#include <string>


class Hllvm {
public:
    Hllvm(){
        moduleInitialize();
        setupExternalFunctions();
    }

    void execute(const std::string& code){

        
        syntax::EvaParser parser;
        auto ast = parser.parse("(begin " + code + ")");

        createGlobalVariable("x", builder->getInt32(42));
        
        compile(ast);

        module->print(llvm::outs(), nullptr);

        saveModuleToFile("./output.ll");
    }
private:
std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::Module> module;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::raw_ostream> output_stream;

llvm::Function* fn;

void moduleInitialize(){
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("Hllvm", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
}  

void saveModuleToFile(const std::string& filename){
    std::error_code error_code;
    llvm::raw_fd_ostream output_stream(filename, error_code);
    module->print(output_stream, nullptr);
}


void setupExternalFunctions(){
    auto printf_type = llvm::FunctionType::get(
        builder->getInt32Ty(), builder->getInt8PtrTy(), true);
    module->getOrInsertFunction("printf", printf_type);
}

llvm::GlobalVariable* createGlobalVariable(const std::string& name, llvm::Constant* value){

    module->getOrInsertGlobal(name, value->getType());
    auto global = module->getGlobalVariable(name);
    global->setAlignment(llvm::Align(4));
    global->setConstant(true);
    global->setInitializer(value);

    return global;
}

void compile(const Exp& ast){
    //create main function
    fn = createFunction("main",
        llvm::FunctionType::get(builder->getInt32Ty(), false));



    // compile the main function
    auto result = gen(ast);

    // cast to i32 return from main function
    // auto i32_return = builder->CreateIntCast(result, builder->getInt32Ty(), true);

    // return the result
    builder->CreateRet(builder->getInt32(0));
} 




llvm::Value* gen(const Exp& ast){

    switch (ast.type) {
        case ExpType::NUMBER:
            return builder->getInt32(ast.number);

        case ExpType::STRING:
            return builder->CreateGlobalStringPtr(ast.string);

        case ExpType::SYMBOL:

            if(ast.string == "true" || ast.string == "false"){
                return builder->getInt1(ast.string == "true"? true : false);
            }

            return module->getNamedGlobal(ast.string)->getInitializer();

        case ExpType::LIST:
            auto first = ast.list[0];

            if (first.string == "var"){ 
                auto var_name = ast.list[1].string;
                auto var_value = gen(ast.list[2]);
                createGlobalVariable(var_name, (llvm::ConstantInt*)var_value);
            }

            llvm::Value* blockRes = nullptr;
            if (first.string == "begin"){
                for (size_t i = 1; i < ast.list.size(); i++){
                     blockRes = gen(ast.list[i]);
                }
                return blockRes;
            }

            else if(first.string == "printf"){
                auto printf_fn = module->getFunction("printf");
                std::vector<llvm::Value*> args = {};
                for (size_t i = 1; i < ast.list.size(); i++){
                    args.push_back(gen(ast.list[i]));
                }
                return builder->CreateCall(printf_fn, args);
            }
            break;
        }
    
    return builder->getInt32(0);
}

llvm::Function* createFunction(const std::string& name, llvm::FunctionType* type){
    auto fn = module->getFunction(name);
    if (!fn){
        fn = createFunctionPrototype(name, type);
    }

    createFunctionBody(fn);

    return fn;
}

llvm::Function* createFunctionPrototype(const std::string& name, llvm::FunctionType* type){
    auto fn = llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, *module);

    llvm::verifyFunction(*fn);

    return fn;
}


void createFunctionBody(llvm::Function* fn){

    auto entry = createBB("entry", fn);
    builder->SetInsertPoint(entry);
}

llvm::BasicBlock* createBB(const std::string& name, llvm::Function* fn = nullptr){
    return llvm::BasicBlock::Create(*context, name, fn);
} 

};

#endif // HLLVM_H   
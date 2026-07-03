#ifndef HLLVM_H
#define HLLVM_H


#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <parser/EvaGrammer.h>

#include <memory>
#include <string>


class Hllvm {
public:
    Hllvm(){
        moduleInitialize();
        setupExternalFunctions();
    }

    void execute(const std::string& code){
        (void)code;
        compile();

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

void compile(){
    //create main function
    fn = createFunction("main",
        llvm::FunctionType::get(builder->getInt32Ty(), false));



    // compile the main function
    auto result = gen(/*ast*/);

    // cast to i32 return from main function
    // auto i32_return = builder->CreateIntCast(result, builder->getInt32Ty(), true);

    // return the result
    builder->CreateRet(builder->getInt32(0));
} 




llvm::Value* gen(/*ast*/){
    auto result = builder->CreateGlobalStringPtr("Hello, World!\n");

    auto printf_fn = module->getFunction("printf");
    std::vector<llvm::Value*> args = {result};
    return builder->CreateCall(printf_fn, args);
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
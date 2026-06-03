#ifndef HLLVM_H
#define HLLVM_H


#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>


class Hllvm {
public:
    Hllvm(){
        moduleInitialize();
    }

    void execute(const std::string& code){
        

        module->print(llvm::outs(), nullptr);

        saveModuleToFile("./output.ll");

    }
private:
std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::Module> module;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::raw_ostream> output_stream;

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


};

#endif // HLLVM_H
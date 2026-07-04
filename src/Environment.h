#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

class Environment {
public:
    Environment(
        std::shared_ptr<Environment> parent = nullptr,
        std::unordered_map<std::string, llvm::GlobalVariable*> variables = {}
    ): parent(parent), variables(variables) {}
    ~Environment();

    void define(const std::string& name, llvm::GlobalVariable* value){
        variables[name] = value;
    }

    llvm::GlobalVariable* lookup(const std::string& name){
       resolve(name)->variables[name]->_
    }
    
Private:    
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, llvm::GlobalVariable*> reolve_;
};

#endif
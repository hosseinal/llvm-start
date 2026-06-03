#include "src/Hllvm.h"

#include <string>


int main(){
    Hllvm hllvm;

    std::string code = "print('Hello, World!');";
    
    hllvm.execute(code);
    
    return 0;
}
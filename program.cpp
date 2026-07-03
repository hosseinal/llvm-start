#include "src/Hllvm.h"

#include <string>


int main(){
    Hllvm hllvm;

    std::string code = "42";
    
    hllvm.execute(code);
    
    return 0;
}
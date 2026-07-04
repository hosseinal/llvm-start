#include "src/Hllvm.h"

#include <string>


int main(){
    Hllvm hllvm;

    std::string code = R"(
        (var xx 48)
        (printf "X: %d\n" xx)
      )";

    hllvm.execute(code);
    
    return 0;
}
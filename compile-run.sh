clang++ -o program `llvm-config --cxxflags --ldflags --system-libs --libs core` program.cpp

./program

"$(llvm-config --bindir)/lli" output.ll

lli output.ll

echo $?
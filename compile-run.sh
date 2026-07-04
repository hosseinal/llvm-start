clang++ -o program $(llvm-config --cxxflags --ldflags --system-libs --libs core) -fexceptions program.cpp

./program

# "$(llvm-config --bindir)/lli" output.ll

lli output.ll

echo $?
#include <iostream>
#include <memory>

#include "compiler.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  auto compiler = std::make_unique<Compiler>();
  //compiler->TranspileC(argv[1]);
  compiler->CompileX64(argv[1]);
}

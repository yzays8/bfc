#include <unistd.h>
#include <iostream>
#include <memory>

#include "compiler.hpp"

int main(int argc, char** argv) {
  opterr = 0;
  int opt;
  auto compiler = std::make_unique<Compiler>();

  while ((opt = getopt(argc, argv, "c")) != -1) {
    switch (opt) {
      case 'c':
        if (argc - optind != 1) {
          std::cerr << "Usage: " << argv[0] << " [-c] <file_path>" << std::endl;
          return 1;
        }
        compiler->TranspileC(argv[optind]);
        return 0;
      default:
        std::cerr << "Usage: " << argv[0] << " [-c] <file_path>" << std::endl;
        return 1;
    }
  }

  if (optind >= argc) {
    std::cerr << "Usage: " << argv[0] << " [-c] <file_path>" << std::endl;
    return 1;
  }

  compiler->CompileX64(argv[1]);
}

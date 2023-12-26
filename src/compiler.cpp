#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <fstream>

#include "compiler.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "gen.hpp"

Compiler::Compiler()
    : lexer_{std::make_unique<Lexer>()},
      parser_{std::make_unique<Parser>()},
      generator_{std::make_unique<Generator>()} {}

std::ifstream Compiler::Load(const std::string& path) {
  namespace fs = std::filesystem;
  if (!fs::is_regular_file(path)) {
    std::cerr << fs::weakly_canonical(fs::absolute(path)) << " is not a regular file" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::ifstream ifs(path, std::ios::binary | std::ios::in);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open " << fs::weakly_canonical(fs::absolute(path)) << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return ifs;
}

void Compiler::TranspileC(const std::string& path) {
  generator_->PrintC(parser_->Parse(lexer_->Lex(Load(path))));
}

void Compiler::CompileX64(const std::string& path) {
  generator_->PrintX64(parser_->Parse(lexer_->Lex(Load(path))));
}

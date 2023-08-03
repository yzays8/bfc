#pragma once

#include <memory>
#include <string>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "gen.hpp"

class Compiler {
 public:
  Compiler();
  void TranspileC(const std::string& path);

 private:
  std::ifstream Load(const std::string& path);

  std::unique_ptr<Lexer> lexer_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<Generator> generator_;
};

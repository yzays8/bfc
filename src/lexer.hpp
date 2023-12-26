#pragma once

#include <string>
#include <list>
#include <fstream>

#include "bfc.hpp"

class Lexer {
 public:
  Lexer();
  std::list<std::unique_ptr<Token>> Lex(std::ifstream ifs);
};

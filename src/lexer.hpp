#pragma once

#include <string>
#include <list>
#include <fstream>

#include "bfc.hpp"

class Lexer {
 public:
  Lexer();
  std::list<std::shared_ptr<Token>>& Lex(std::ifstream ifs);

 private:
  std::list<std::shared_ptr<Token>> tokens_;
};
